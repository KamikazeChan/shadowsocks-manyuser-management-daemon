<?php

use WHMCS\Database\Capsule;

define("CLIENTAREA", true);
//加载配置文件
require("init.php");
require("configuration.php");

$ca = new WHMCS_ClientArea();
//设定页面标题
$ca->setPageTitle("shadowsocks设置");
//设定breadcrumb
$ca->addToBreadCrumb('index.php', Lang::trans('globalsystemname'));
$ca->addToBreadCrumb('ss_change_setting.php', 'shadowsocks设置');
//初始化页面
$ca->initPage();

//定义变量，本文件中全局
$encrypt_method_options = array(    //加密方式
    'table' => 'table',
    'rc4' => 'rc4',
    'rc4-md5' => 'rc4-md5',
    'aes-128-cfb' => 'aes-128-cfb',
    'aes-192-cfb' => 'aes-192-cfb',
    'aes-256-cfb' => 'aes-256-cfb',
    'bf-cfb' => 'bf-cfb',
    'camellia-128-cfb' => 'camellia-128-cfb',
    'camellia-192-cfb' => 'camellia-192-cfb',
    'camellia-256-cfb' => 'camellia-256-cfb',
    'cast5-cfb' => 'cast5-cfb',
    'des-cfb' => 'des-cfb',
    'idea-cfb' => 'idea-cfb',
    'rc2-cfb' => 'rc2-cfb',
    'seed-cfb' => 'seed-cfb',
    'salsa20' => 'salsa20',
    'chacha20' => 'chacha20',
);
$udp_relay_options = array( //udp转发
    0 => 'OFF',
    1 => 'ON',
);
$fast_open_options = array( //快速连接
    0 => 'OFF',
    1 => 'ON',
);

if ($ca->isLoggedIn()) {    //判断是否登陆
    //用户已登陆
    $hostingid = $_GET['id'];    //获取参数,hostingid即serviceid，唯一
    if (!is_numeric($hostingid))    //使用非法字符
        die("你们啊，不要想搞一个大新闻");
    //判断是否拥有该产品
    $mysql = mysql_connect($db_host, $db_username, $db_password);
    if (!$mysql)   //数据库连接失败
        die("whmcs数据库繁忙，请稍后重试");
    mysql_select_db($db_name, $mysql);
    $userid = $_SESSION['uid'];   //获取用户id
    $query = mysql_query("SELECT * FROM `tblhosting` WHERE `userid`= " . $userid . " AND `id`= " . $hostingid, $mysql);  //查询该用户是否拥有该产品
    $result = mysql_fetch_array($query);
    if (!$result)  //返回结果为空
        die("你不是该产品的所有者");
    $packageid = $result['packageid'];  //获取产品id
    $domainstatus = $result['domainstatus'];  //获取产品状态
    if ($domainstatus != "Active")   //产品当前不为Active状态
        die("产品当前不可用");

    $query = mysql_query("SELECT * FROM `tblproducts` WHERE `id` = " . $packageid, $mysql);   //获取该产品设置
    $result = mysql_fetch_array($query);
    $serverid = $result['gid'];   //服务器id
    $ss_db_database = $result['configoption1'];    //ss用户数据库
    $ss_db_table = $result['configoption2']; //ss用户表

    $query = mysql_query("SELECT * FROM `tblservers` WHERE `id` = " . $serverid, $mysql);   //获取服务器设置
    $result = mysql_fetch_array($query);
    $ss_db_host = $result['ipaddress'];
    $ss_db_username = $result['username'];
    $ss_db_password = decrypt($result['password'], $cc_encryption_hash);    //decrypt是whmcs内置函数，key在whmcs配置文件中

    mysql_close($mysql);    //关闭当前mysql连接并连接ss数据库服务器
    $mysql = mysql_connect($ss_db_host, $ss_db_username, $ss_db_password);
    if (!$mysql)   //数据库连接失败
        die("shadowsocks数据库繁忙，请稍后重试");
    mysql_select_db($ss_db_database, $mysql);
    $query = mysql_query("SELECT * FROM `" . $ss_db_table . "` WHERE `pid` = " . $hostingid, $mysql);    //获取ss设置
    $result = mysql_fetch_array($query);
    if (!$result)  //ss表内没有此hostingid
        die("产品不存在");
    $ss_setting_encrypt_method = $result['encrypt_method']; //加密方式
    $ss_setting_udp_relay = $result['udp_relay'];   //udp转发
    $ss_setting_fast_open = $result['fast_open'];   //快速连接
    //获取post内容
    $post_encrypt_method = $_POST['encrypt_method'];
    $post_udp_relay = $_POST['udp_relay'];
    $post_fast_open = $_POST['fast_open'];
    if ($post_encrypt_method != null || $post_udp_relay != null || $post_fast_open != null) {    //所有参数至少有一个不是null
        if ($post_encrypt_method != null && $post_udp_relay != null && $post_fast_open != null) {  //所有参数全部不为null
            $query = mysql_query("UPDATE `" . $ss_db_table . "` SET `encrypt_method` = '" . $post_encrypt_method . "',`udp_relay` = " . $post_udp_relay . ",`fast_open` = " . $post_fast_open . " WHERE `pid` = " . $hostingid, $mysql);
            if ($query)
                $ca->assign('result', "成功");
            else
                $ca->assign('result', "失败");
            $ca->assign('hostingid', $hostingid);    //hostingid
            $ca->setTemplate('ss_setting_change_result');
        } else {    //有参数为null,有参数不为null
            die("参数错误");
        }
    } else {    //所有参数全部为null,此时不处理post事务，显示ss设置
        //传入tpl
        $ca->assign('hostingid', $hostingid);    //hostingid
        $ca->assign('encrypt_method_options', $encrypt_method_options);  //select选项，传入值为array
        $ca->assign('ss_setting_encrypt_method', $ss_setting_encrypt_method);   //默认被选中的select项
        $ca->assign('udp_relay_options', $udp_relay_options);
        $ca->assign('ss_setting_udp_relay', $ss_setting_udp_relay);
        $ca->assign('fast_open_options', $fast_open_options);
        $ca->assign('ss_setting_fast_open', $ss_setting_fast_open);

        $ca->setTemplate('ss_setting'); //设定为ss_setting.tpl
    }
} else {
    //用户未登录
    $ca->setTemplate('login');  //设定为login.tpl
}
$ca->output();  //显示tpl
