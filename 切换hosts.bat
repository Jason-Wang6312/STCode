@echo off

cd.>C:\Windows\System32\drivers\etc\hosts

echo 0.切换到本地 1.切换124测试环境  2.切换122环境  3.切换164官网服务器地址  4.切换到批发站202仿生产  5.切换到批发站123测试环境

set /p user_input=请输入0/1/2/3/4/5，切换Hosts文件内容：

if %user_input% equ 0 (
echo 127.0.0.1    www.k.com
echo 127.0.0.1    wap.k.com
echo 127.0.0.1    m.k.com
echo 127.0.0.1    app.k.com
echo 127.0.0.1    buy.k.com
echo 127.0.0.1    user.k.com
echo 127.0.0.1    api.k.com
echo 127.0.0.1    wx.k.com
echo 127.0.0.1    up.k.com
echo 127.0.0.1    admin.k.com
)>>C:\Windows\System32\drivers\etc\hosts


if %user_input% equ 1 (
echo 192.168.1.124    www.chunshuitang.com
echo 192.168.1.124    wap.chunshuitang.com
echo 192.168.1.124    m.chunshuitang.com
echo 192.168.1.124    app.chunshuitang.com
echo 192.168.1.124    buy.chunshuitang.com
echo 192.168.1.124    user.chunshuitang.com
echo 192.168.1.124    api.chunshuitang.com
echo 192.168.1.124    wx.chunshuitang.com
echo 192.168.1.124    up.chunshuitang.com
echo 192.168.1.124    admin.chunshuitang.com
)>>C:\Windows\System32\drivers\etc\hosts

if %user_input% equ 2 (
echo 192.168.1.122    www.chunshuitang.com
echo 192.168.1.122    wap.chunshuitang.com
echo 192.168.1.122    m.chunshuitang.com
echo 192.168.1.122    app.chunshuitang.com
echo 192.168.1.122    buy.chunshuitang.com
echo 192.168.1.122    user.chunshuitang.com
echo 192.168.1.122    api.chunshuitang.com
echo 192.168.1.122    wx.chunshuitang.com
echo 192.168.1.122    up.chunshuitang.com
echo 192.168.1.122    admin.chunshuitang.com
)>>C:\Windows\System32\drivers\etc\hosts

if %user_input% equ 3 (
echo 120.131.68.164    www.chunshuitang.com
echo 120.131.68.164    wap.chunshuitang.com
echo 120.131.68.164    m.chunshuitang.com
echo 120.131.68.164    app.chunshuitang.com
echo 120.131.68.164    buy.chunshuitang.com
echo 120.131.68.164    user.chunshuitang.com
echo 120.131.68.164    api.chunshuitang.com
echo 120.131.68.164    wx.chunshuitang.com
echo 120.131.68.164    up.chunshuitang.com
echo 120.131.68.164    static.chunshuitang.com
echo 120.131.68.164    admin.chunshuitang.com
)>>C:\Windows\System32\drivers\etc\hosts

if %user_input% equ 4 (
echo 192.168.100.202    www.chunshuitang.com
echo 192.168.100.202    wap.chunshuitang.com
echo 192.168.100.202    m.chunshuitang.com
echo 192.168.100.202    app.chunshuitang.com
echo 192.168.100.202    buy.chunshuitang.com
echo 192.168.100.202    vc.chunshuitang.com
echo 192.168.100.202    user.chunshuitang.com
echo 192.168.100.202    api.chunshuitang.com
echo 192.168.100.202    wx.chunshuitang.com
echo 192.168.100.202    up.chunshuitang.com
echo 192.168.100.202    vcadm.chunshuitang.com
echo 192.168.100.202	vc.chunshuitang.com
)>>C:\Windows\System32\drivers\etc\hosts

if %user_input% equ 5 (
echo 192.168.1.123    www.chunshuitang.com
echo 192.168.1.123    wap.chunshuitang.com
echo 192.168.1.123    m.chunshuitang.com
echo 192.168.1.123    app.chunshuitang.com
echo 192.168.1.123    buy.chunshuitang.com
echo 192.168.1.123    vc.chunshuitang.com
echo 192.168.1.123    user.chunshuitang.com
echo 192.168.1.123    api.chunshuitang.com
echo 192.168.1.123    wx.chunshuitang.com
echo 192.168.1.123    up.chunshuitang.com
echo 192.168.1.123    vcadm.chunshuitang.com
echo 192.168.1.123    vc.chunshuitang.com
)>>C:\Windows\System32\drivers\etc\hosts

echo 切换成功

pause



