Model_version_2_2022: Model.o File_Management.o Dynamic.o Entities.o
	g++ -g -O3 ./App/Obj/Model.o ./App/Obj/File_Management.o ./App/Obj/Dynamic.o ./App/Obj/Entities.o -o ./App/Model_version_2_2022 -std=c++17 

Model.o: Model.cpp
	g++ -g -O3 -c ./Model.cpp -I ./Include -o ./App/Obj/Model.o -std=c++17

File_Management.o: File_Management.cpp
	g++ -g -O3 -c ./File_Management.cpp -I ./Include -o ./App/Obj/File_Management.o -std=c++17 

Dynamic.o: Dynamic.cpp
	g++ -g -O3 -c ./Dynamic.cpp -I ./Include -o ./App/Obj/Dynamic.o -std=c++17 

Entities.o: Entities.cpp
	g++ -g -O3 -c ./Entities.cpp -I ./Include -o ./App/Obj/Entities.o -std=c++17 