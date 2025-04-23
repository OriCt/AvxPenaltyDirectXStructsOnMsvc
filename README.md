This repo contains a sample code ilustrates optimization bug reproduced when
using DirectX::XMFLOAT3 derived class while adding /arch:AVX2 in optimize 
compilation (/O2 /DNDEBUG).
When comparing performance of the code compiled with /arch:AVX2 flag to compilation witout the flag
there is performance penalty with a factor of 2.5-3.

From the assembly analysis it seems there is a redundant memory usage when /arch:AVX2 is set:

Assembly MSVC (without /arch:AVX2):

rrVec\[i\] += addVal;
00007FF7110C1330  movsd       xmm2,mmword ptr \[r9+r8\]  
for (int i = 0; i \< arrVec.size() - 1; i++) {
00007FF7110C1336  inc         r10d  
arrVec\[i\] += addVal;
00007FF7110C1339  movss       xmm0,dword ptr \[r9+r8+8\]  
for (int i = 0; i \< arrVec.size() - 1; i++) {
00007FF7110C1340  mov         rax,r15  
arrVec\[i\] += addVal;
00007FF7110C1343  movlhps     xmm2,xmm0  
00007FF7110C1346  addps       xmm2,xmm3  
00007FF7110C1349  movaps      xmm0,xmm2  
00007FF7110C134C  shufps      xmm2,xmm2,0AAh  
00007FF7110C1350  movsd       mmword ptr \[r9+r8\],xmm0  
00007FF7110C1356  movss       dword ptr \[r9+r8+8\],xmm2

Assembly MSVC + /arch:AVX2:

arrVec\[i\] += addVal;
00007FF7D2231330  vmovsd      xmm1,qword ptr \[r9+r8\]  
00007FF7D2231336  vinsertps   xmm2,xmm1,dword ptr \[r9+r8+8\],20h  
00007FF7D223133E  vaddps      xmm3,xmm2,xmm4  
00007FF7D2231342  vextractps  eax,xmm3,2  
for (int i = 0; i \< arrVec.size() - 1; i++) {
00007FF7D2231348  inc         r10d  
00007FF7D223134B  lea         r9,\[r9+0Ch\]  
arrVec\[i\] += addVal;
00007FF7D223134F  vextractps  dword ptr [addVal],xmm3,0  
00007FF7D2231357  vextractps  dword ptr [rsp+24h],xmm3,1  
00007FF7D223135F  vmovsd      xmm0,qword ptr [rsp+20h]  
00007FF7D2231365  vmovsd      qword ptr [r9+r8-0Ch],xmm0  
00007FF7D223136C  mov         dword ptr [r9+r8-4],eax


To run the repo and witness the bug:
1)Install visual studio 2022.
2)Open x64 Native Tools Command Prompt for VS 2022
3)Run:cl /arch:AVX2 /EHsc /FAs /O2 /DNDEBUG PathToFile\Source.cpp
4)Run:Source.exe
5)An average benchmark time should apear: Average Elapsed time...
6)dumpbin.exe /DISASM Source.exe >dumpbin_avx.log (to save assembly)
7)Run:cl /EHsc /FAs /O2 /DNDEBUG PathToFile\Source.cpp
8)Run:Source.exe
9)An average benchmark time should apear (should be 2-3 times faster than previous run).
10)dumpbin.exe /DISASM Source.exe >dumpbin_sse.log (to save assembly)
