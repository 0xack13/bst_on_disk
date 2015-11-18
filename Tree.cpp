#include "Tree.h"
#define ROOT_OFFSET 0
#define VALUE 0
#define L_OFFSET 1
#define R_OFFSET 2
#define LINE_LENGTH 16

Tree::Tree( std::string fileName )
: root_(), fd_( open( fileName.c_str(), O_RDWR|O_CREAT|O_TRUNC, 0666) )
{
}
Tree::~Tree() { close(fd_); }
Tree::Tree( const Tree& rhs ) {}
Tree& Tree::operator=( const Tree& rhs ) {}
Tree::Tree( Tree& rhs ) {}
Tree& Tree::operator=( Tree& rhs ) {}
void Tree::insertNode( int val )
{
	char buf[5];
	int read_bytes;
	lseek( fd_, 0, SEEK_SET ); //Seek to start of file
	if( ( read_bytes = read( fd_, &buf, 5) ) > 0 ) //Check for empty file
	{
		insertNode( ROOT_OFFSET, val ); //File not empty
	}
	else
	{
		std::string new_node = createNode( val ); //Create root node
		int offset = writeLine( new_node ); //Write root node
	}
}
void Tree::insertNode( int offset, int val )
{
	lseek( fd_, 0, offset ); //Seek to node
	std::string line = getLine( offset ); //get entry for node from file
	std::vector<std::string> node = parseLine( line ); //Extract value and offsets from node
	int node_val = std::stoi(node[VALUE]); //value
	int node_loff = std::stoi(node[L_OFFSET]); //left child offset
	int node_roff = std::stoi(node[R_OFFSET]); //right child offset 
	if( val < node_val )
	{
		if( node_loff == 0 ) //no left child
		{
			std::string new_node = createNode( val );
			int new_node_offset = writeLine( new_node );
			//change left offset value in parent to point to child
			setNode( offset, new_node_offset, L_OFFSET, node );
		}
		else
		{
			insertNode( node_loff, val );			
		}
	}
	else
	{
		if( node_roff == 0 ) //no right child
		{
			std::string new_node = createNode( val );
			int new_node_offset = writeLine( new_node );
			//change right offset value in parent to point to child
			setNode( offset, new_node_offset, R_OFFSET, node );
		}
		else
		{
			insertNode( node_roff, val );
		}

	}
}
void Tree::merge( const char * fileName )
{
	int fd2=open( fileName, O_RDWR, 0666);
	mergeTree( fd2, ROOT_OFFSET );
}
void Tree::mergeTree( int fd2, int offset )
{

	lseek( fd2, offset, SEEK_SET );
	std::string line = getLine( fd2, offset ); 
	std::vector<std::string> node = parseLine( line );
	insertNode(std::stoi(node[VALUE]));
	if( std::stoi(node[L_OFFSET]) )
	{
		mergeTree( fd2, std::stoi(node[L_OFFSET]) );
	}
	if( std::stoi(node[R_OFFSET]) )
	{
		mergeTree( fd2, std::stoi(node[R_OFFSET]) );
	}
	
}
void Tree::search( int val )
{
	search( val, ROOT_OFFSET );
}
void Tree::search( int val,int offset )
{
	
	lseek( fd_, offset, SEEK_SET );
	std::string line = getLine( offset ); 
	std::vector<std::string> node = parseLine( line );
	int node_loffset=std::stoi(node[L_OFFSET]);
	int node_roffset=std::stoi(node[R_OFFSET]);
	int node_val=std::stoi(node[VALUE]);
	if( (val<node_val) && (node_loffset) )
	{
		search( val, node_loffset );
	}
	else if( (val>node_val) && (node_roffset) )
	{
		search( val, node_roffset );
	}
	else if( val==node_val )
	{
		std::cout<<"Search element "<<val<<" is present"<<"\n";
		return;
	}
	else
	{
		std::cout<<"Search element "<<val<<" not present"<<"\n";
		return;
	}
}
void Tree::setNode( int offset, int new_value, int pos, std::vector<std::string> node )
{
	node[pos] = std::to_string(new_value); //set the new value
	//Create string for the node
	std::string line;
	line += node[VALUE];
	line += ":";
	line += node[L_OFFSET];
	line += ":";
	line += node[R_OFFSET];
	//line += "\n";
	//write the string back to the node position
	writeLine( offset, line );
}
std::string Tree::createNode( int val )
{
	//Create a string for a new node
	std::string node;
	node += std::to_string(val);
	node += ":"; //Delimiter for val
	node += std::to_string(0);
	node += ":"; //Delimiter for offset
	node += std::to_string(0);
	node += "\n"; //Delimiter for node entry
	return node;
}
int Tree::writeLine( std::string new_node )
{
	//Append line to the end of the file
	int off = lseek( fd_, 0, SEEK_END );
	char node_arr[new_node.length()]; //Create array
	strncpy(node_arr, new_node.c_str(), sizeof(node_arr)); //Copy string onto array
	write( fd_, &node_arr, sizeof(node_arr) ); //Write the array
    char buf[10] = { '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0' };
	write( fd_, &buf, 10 );
	return off;
}
int Tree::writeLine( int offset, std::string new_node )
{
	//Write Line to given offset
	int off = lseek( fd_, offset, SEEK_SET );
	char node_arr[new_node.length()+1]; //Create array 
	strncpy(node_arr, new_node.c_str(), sizeof(node_arr)); //Copy string onto array 
    node_arr[sizeof(node_arr)-1] = '\n';
	write( fd_, &node_arr, sizeof(node_arr) ); //Write the array
    char buf = '\0';
    for( int i = 0; i < LINE_LENGTH - sizeof(node_arr); ++i )
        write( fd_, &buf, 1 );
    //char buf[10] = { '\0','\0','\0','\0','\0','\0','\0','\0','\0','\0' };
	//write( fd_, &buf, 10 );
	return off;
	
}
std::string Tree::getLine( int offset )
{
	lseek( fd_, offset, SEEK_SET );
	char buf = '\0'; //Default value
	std::string line;
	int read_bytes;
	while( buf != '\n' ) //Read till new line character (delimiter for the node entry)
	{
		if( ( read_bytes = read( fd_, &buf, 1 ) ) != 1 )
		{
			std::cerr << "Read failed\n";
			exit(1);
		}
		line += buf; //Buffer the node entry
	}
    line.pop_back();
	return line;
}
std::string Tree::getLine(int fd2, int offset )
{
	lseek( fd2, offset, SEEK_SET );
	char buf = '\0'; //Default value
	std::string line;
	int read_bytes;
	while( buf != '\n' ) //Read till new line character (delimiter for the node entry)
	{
		if( ( read_bytes = read( fd2, &buf, 1 ) ) != 1 )
		{
			std::cerr << "Read failed\n";
			exit(1);
		}
		line += buf; //Buffer the node entry
	}
    line.pop_back();
	return line;
}

std::vector<std::string> Tree::parseLine( std::string node )
{
    int pos1 = node.find_first_of(":");
    int pos2 = node.find_last_of(":");
    std::string val = node.substr(0,pos1); //Substring of value
    std::string loffset = node.substr(pos1+1,pos2-pos1-1); //Substring of left offset
    std::string roffset = node.substr(pos2+1,std::string::npos); //Substring of right offset
    return std::vector<std::string>{val,loffset,roffset}; //Return array of three stings
}

void Tree::display()
{
    display( ROOT_OFFSET );
}

void Tree::display( int offset )
{
	lseek( fd_, offset, SEEK_SET );
	std::string line = getLine( offset ); 
    std::cout << line << '\n';
	std::vector<std::string> node = parseLine( line );
	if( std::stoi(node[L_OFFSET]) )
	{
	    display( std::stoi(node[L_OFFSET]) );
	}
	if( std::stoi(node[R_OFFSET]) )
	{
		display( std::stoi(node[R_OFFSET]) );
	}
}
