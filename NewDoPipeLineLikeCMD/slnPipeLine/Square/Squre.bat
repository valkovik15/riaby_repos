echo Square(Square(Floats.txt)) > Results.txt
echo Square(Square(Floats.txt)) > Errors.txt
..\Debug\Square.exe < Floats.txt  2>>Errors.txt |..\Debug\Square.exe >> Results.txt

echo Square(Square(Square(Floats5.txt))) >> Results.txt
echo Square(Square(Square(Floats5.txt))) >> Errors.txt
..\Debug\Square.exe < Floats5.txt  2>>Errors.txt |..\Debug\Square.exe |..\Debug\Square.exe >> Results.txt

echo Sum(Square(Floats5.txt)) >> Results.txt
echo Sum(Square(Floats5.txt)) >> Errors.txt
..\Debug\Square.exe < Floats5.txt  2>>Errors.txt |..\Debug\Sum.exe >> Results.txt

REM End of Job
pause