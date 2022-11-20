echo "-----Step 1---------------"
echo "Building utlibcutil..."
gcc -g -fPIC utilities/sources/common.c  -shared -o utilities/build/libcutil.so
echo "Building utlibcutil done"
echo "Coping utlibcutil.so server and client..."
cp utilities/build/utlibcutil.so  server/libs/libcutil.so
cp utilities/build/utlibcutil.so  chat_client/libs/libcutil.so
echo "Coping utlibcutil.so server and client done"

echo "-----Step 2---------------"
echo "compiling server..."
gcc -I utilities/ -c server/main.c -o server/main.o 
echo "Compiling server done"

echo "linking server..."
gcc -o server/build/server server/main.o -Lutilities/build -lcutil -lpthread
echo "linking server done"

echo "-----Step 3---------------"
echo "compiling client..."
gcc -I utilities/ -c chat_client/main.c -o chat_client/main.o 
echo "Compiling client done"

echo "Loading client..."
gcc -o chat_client/build/client chat_client/main.o -Lutilities/build -lcutil
echo "Loading client done"


echo "Builing done......"


