# BypassAV
Implementing bypass through patching legitimate files, without adding other evasion techniques. May become ineffective relatively quickly. You can create new versions based on the shellcode template.

If you find my project useful, please leave a star. It will motivate me to update more frequently.

## Version Updates

<img src="https://img.shields.io/github/downloads/Sakura529/BypassAV/total?style=flat" alt="GitHub Downloads (all assets, all releases)"/>

~Version 0.3 has issues and has been temporarily removed~

Re-released

0.3 Changed the legitimate program, modified patch method, replaced call chain, achieved full green on VT

0.2 Modified resources, bypass 360qvm

0.1

## Usage

Use the donut tool with command `donut.exe -i mimikatz.exe -o 123.bin` to convert post-exploitation tools to shellcode,

Then use the sgn tool with command `sgn.exe -i 123.bin -o work.bin` for encryption, naming it work.bin.

Then place work.bin in the same directory as the binary program from the release, and run the binary program.

The original exe cannot be encrypted or packed, such as the release version of fscan, which cannot be converted to shellcode for use.

This tool is limited to legal penetration testing only. Do not use it for illegal activities. Any damages caused by this tool shall be borne by the user.

## Testing

360:
<img width="1133" height="800" alt="Screenshot 2025-12-17 093934" src="https://github.com/user-attachments/assets/07a848e9-718d-4366-822b-357e869baccb" />

Huorong:
<img width="1052" height="684" alt="Screenshot 2025-12-17 093951" src="https://github.com/user-attachments/assets/95e22ac5-177a-4135-8a5f-ed8d196fa74a" />

Defender:
<img width="1095" height="625" alt="Screenshot 2025-12-17 094237" src="https://github.com/user-attachments/assets/53d84602-e22d-4a03-be47-c63b8d11b1d5" />

Kaspersky:
<img width="1247" height="779" alt="Screenshot 2025-12-17 093919" src="https://github.com/user-attachments/assets/c40a05a0-082d-45a2-97b9-54d742a3217f" />

VT Scan:
<img width="2022" height="1266" alt="image" src="https://github.com/user-attachments/assets/a3d76a35-003d-44fe-8ab9-4d055638c6dd" />


References:

https://xz.aliyun.com/news/14518

https://www.52pojie.cn/thread-1900852-1-1.html

https://github.com/yinsel/BypassAV

https://github.com/yj94/BinarySpy?tab=readme-ov-file

https://github.com/clownfive/CppDevShellcode
