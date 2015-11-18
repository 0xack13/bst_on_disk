#include <iostream>
#include "Tree.h"

int main( int argc, char *argv[] )
{
    if( argc < 2 )
    {
        std::cerr << "Usage : " << argv[0] << " [FILENAME] [MERGE_FILES] .. " << '\n';
        exit(EXIT_FAILURE);
    }
    Tree t( argv[1] );
	/*t.insertNode(50);
	t.insertNode(19);
	t.insertNode(12);
    t.insertNode(6);
    t.insertNode(21);
    t.insertNode(45);
    t.insertNode(7);
    t.insertNode(13);*/
    int ch;
    bool nextInput = true;
    while( nextInput )
    { 
        std::cout << "Enter choice : 1. Insert; 2. Search 3. Merge 4. Display 5. Exit\n";
        std::cin >> ch;
        int val;
        int j;
        switch( ch )
        {
            case 1: std::cout << "Enter value : "; std::cin >> val;  t.insertNode( val ); break;
            case 2: std::cout << "Enter value : "; std::cin >> val;  t.search( val ); break;
            case 3: for( int j = 2; j < argc; ++j ){ t.merge( argv[j] ); } break;
            case 4: t.display(); break;
            case 5: nextInput = false; break;
        }
    }
	/*t.merge("tree2.txt");
	t.search(2);
	t.search(7);*/
	return 0;
}
