页面测试中，有bug请报告<br><br>
<strong>若您不具备相关专业知识，请不要修改以下设置</strong><br>
<form action="/ss_change_setting.php?id={$hostingid}" method="post">
    <p>
        加密方式<br>
        <select name="encrypt_method" id="encrypt_method">
            {html_options options=$encrypt_method_options selected=$ss_setting_encrypt_method}
        </select><br>
        *注意，此选项需要客户端设备支持，否则无法使用
    </p>
    <p>
        UDP转发<br>
        <select name="udp_relay" id="udp_relay">
            {html_options options=$udp_relay_options selected=$ss_setting_udp_relay}
        </select><br>
        *注意，UDP转发在一定情况下可能导致访问故障
    </p>
    <p>
        TCP快速连接<br>
        <select name="fast_open" id="fast_open">
            {html_options options=$fast_open_options selected=$ss_setting_fast_open}
        </select><br>
        *注意，此选项仅适用于LINUX3.7以上
    </p>
    <p>
        <input type="submit" name="submit" id="submit" value="提交"/>
    </p>
</form>
