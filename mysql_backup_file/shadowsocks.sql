SET FOREIGN_KEY_CHECKS = 0;

CREATE TABLE `free_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `port` int(11) NOT NULL,
  `passwd` varchar(16) NOT NULL,
  `encrypt_method` varchar(32) NOT NULL DEFAULT 'aes-256-cfb',
  `udp_relay` tinyint(1) NOT NULL DEFAULT '0',
  `fast_open` tinyint(1) NOT NULL DEFAULT '0',
  `group` varchar(32) NOT NULL,
  `enable` tinyint(1) NOT NULL DEFAULT '1',
  `pid` varchar(50) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `index_port` (`port`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='免费用户';

CREATE TABLE `node_free` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ip` varchar(32) NOT NULL,
  `domain` varchar(256) DEFAULT NULL,
  `area` varchar(32) DEFAULT NULL,
  `os` varchar(32) DEFAULT NULL,
  `net_load_5min` varchar(32) DEFAULT '0',
  `net_load_10min` varchar(32) DEFAULT '0',
  `net_load_15min` varchar(32) DEFAULT '0',
  `group` varchar(32) NOT NULL,
  `remark` varchar(32) DEFAULT NULL,
  `order` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='免费节点';

SET FOREIGN_KEY_CHECKS = 1;

