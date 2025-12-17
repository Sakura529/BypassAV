# BypassAV
通过Patch白文件实现Bypass,没有添加其他免杀手法，失效可能比较快。可以自行根据shellcode模板创建新版本。
## 使用方法

使用donut将后渗透工具转换为shellcode，再使用sgn进行加密，命名为work.bin即可。
原始exe不能经过加密、加壳，如fscan的发行版，无法转换成shellcode使用。
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


参考链接：

https://xz.aliyun.com/news/14518

https://www.52pojie.cn/thread-1900852-1-1.html

https://github.com/yinsel/BypassAV

https://github.com/yj94/BinarySpy?tab=readme-ov-file

https://github.com/clownfive/CppDevShellcode
