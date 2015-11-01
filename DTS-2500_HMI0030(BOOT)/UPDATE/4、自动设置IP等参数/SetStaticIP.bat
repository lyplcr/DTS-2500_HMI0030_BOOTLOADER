start ncpa.cpl  ::打开网络连接
set addr=192.168.0.73
set mask=255.255.255.0
set gateway=192.168.0.1
set dns=192.168.0.1
netsh interface ip set address name="本地连接" source=static addr=%addr% mask=%mask% gateway=%gateway%
netsh interface ip set dns name="本地连接" source=static addr=%dns%
exit
::注：有时候网关有冲突，可能导致设置失败，因此需进入[本地连接] - [Internet 协议版本4] - 点击确认！方可生效