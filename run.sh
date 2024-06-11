python3 generator2.py $1
make runC VAR="test.asm"
echo "------------------------"
./a.out
A=$?
echo "------------------------"
echo $A