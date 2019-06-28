echo SquareRoot(Sum(Square(Floats5.txt))) > Results5.txt
echo SquareRoot(Sum(Square(Floats5.txt))) > Errors5.txt

..\Debug\Square.exe < Floats5.txt  2>>Errors5.txt |..\Debug\Sum.exe |..\Debug\SqureRoot.exe >> Results5.txt

REM End of Job
pause