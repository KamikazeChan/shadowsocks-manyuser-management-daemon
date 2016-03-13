SET FOREIGN_KEY_CHECKS = 0;

CREATE TABLE `free_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `port` int(11) NOT NULL,
  `passwd` varchar(16) NOT NULL,
  `encrypt_method` varchar(32) NOT NULL DEFAULT 'aes-256-cfb',
  `udp_relay` tinyint(1) NOT NULL DEFAULT '0',
  `fast_open` tinyint(1) NOT NULL DEFAULT '0',
  `group` varchar(32) NOT NULL DEFAULT 'free_user',
  PRIMARY KEY (`id`),
  KEY `index_port` (`port`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='免费用户';

insert into `free_user`(`id`,`port`,`passwd`,`encrypt_method`,`udp_relay`,`fast_open`,`group`)
values('1','20000','czp','aes-256-cfb','0','0','free_user');
insert into `free_user`(`id`,`port`,`passwd`,`encrypt_method`,`udp_relay`,`fast_open`,`group`)
values('2','20001','czp2','aes-256-cfb','0','0','free_user');
SET FOREIGN_KEY_CHECKS = 1;

