Rem Current Directory: <C:\Users\fpm.kopochel\source\repos\NewDoPipeLineLikeCMD\slnPipeLine\DoPipeLine>
cd ..
cd Square
Rem Current Directory: <slnPipeLine\Square\
goto 2
goto 1

..\Debug\DoPipeLine.exe "Square<Floats5 2>Errors2|Sum>Results1"
type Floats5.txt
type Errors2.txt
type Results1.txt
pause
exit
:1
..\Debug\DoPipeLine.exe "Square<Floats 2>Errors2|Square>Results2"
type Floats.txt
type Errors2.txt
type Results2.txt
pause
:2
..\Debug\DoPipeLine.exe "Sum<Floats 2>Errors2|SqureRoot>Results2"
type Floats.txt
type Errors2.txt
type Results2.txt
pause
Rem End of Job
exit
