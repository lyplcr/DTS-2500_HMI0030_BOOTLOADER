start ncpa.cpl  ::����������
set addr=192.168.0.73
set mask=255.255.255.0
set gateway=192.168.0.1
set dns=192.168.0.1
netsh interface ip set address name="��������" source=static addr=%addr% mask=%mask% gateway=%gateway%
netsh interface ip set dns name="��������" source=static addr=%dns%
exit
::ע����ʱ�������г�ͻ�����ܵ�������ʧ�ܣ���������[��������] - [Internet Э��汾4] - ���ȷ�ϣ�������Ч