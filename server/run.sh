echo "Starting compilation..."
gcc  sources/common/common.c main.c -o build/mainserver
echo "Compilation done"
echo "************"
echo "Starting execution..."
./build/mainserver
echo "Done..."