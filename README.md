# BypassAV
通过Patch白文件实现Bypass,没有添加其他免杀手法，失效可能比较快。可以自行根据shellcode模板创建新版本。

## 版本升级
~0.3版本存在问题，暂时删除~

重新发布

0.3 添加syscall，更换白程序，修改patch方法，替换调用链，实现VT全绿

0.2 修改资源，bypass 360qvm

0.1

## 使用方法

使用donut工具通过命令 donut.exe -i mimikatz.exe -o 123.bin 将后渗透工具转换为shellcode，

再使用sgn工具通过命令 sgn.exe -i 123.bin -o work.bin 进行加密，命名为work.bin即可。

然后将work.bin与release中的二进制程序放在同一目录中，运行二进制程序即可。

原始exe不能经过加密、加壳，如fscan的release版本，无法转换成shellcode使用。

本工具仅限用于合法的渗透测试，请勿用于违法行为，因本工具造成的任何损失由使用者自行承担。

## 测试

360：
<img width="1133" height="800" alt="屏幕截图 2025-12-17 093934" src="https://github.com/user-attachments/assets/07a848e9-718d-4366-822b-357e869baccb" />

火绒：
<img width="1052" height="684" alt="屏幕截图 2025-12-17 093951" src="https://github.com/user-attachments/assets/95e22ac5-177a-4135-8a5f-ed8d196fa74a" />

defender：
<img width="1095" height="625" alt="屏幕截图 2025-12-17 094237" src="https://github.com/user-attachments/assets/53d84602-e22d-4a03-be47-c63b8d11b1d5" />

卡巴斯基：
<img width="1247" height="779" alt="屏幕截图 2025-12-17 093919" src="https://github.com/user-attachments/assets/c40a05a0-082d-45a2-97b9-54d742a3217f" />

VT扫描：
<img width="2022" height="1266" alt="image" src="https://github.com/user-attachments/assets/a3d76a35-003d-44fe-8ab9-4d055638c6dd" />


参考链接：

https://xz.aliyun.com/news/14518

https://www.52pojie.cn/thread-1900852-1-1.html

https://github.com/yinsel/BypassAV

https://github.com/yj94/BinarySpy?tab=readme-ov-file

https://github.com/clownfive/CppDevShellcode
