#ifndef TREE_H
#define TREE_H
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
class Tree
{
	public:
		Tree( std::string fileName );
		~Tree();
		Tree( const Tree& rhs );
		Tree& operator=( const Tree& rhs );
		Tree( Tree& rhs );
		Tree& operator=( Tree& rhs );
		void insertNode( int val );
		void search(int val);
		void merge(const char * fileName);
        void display();
	private:
		std::string root_;
		int fd_;
		void mergeTree( int fd2, int offset );
		void search( int val, int offset );
		void insertNode( int offset, int val );
		std::string getLine( int offset );
		std::string getLine( int fd2, int offset );
		std::vector<std::string> parseLine( std::string );
		int writeLine( int offset, std::string new_node );
		int writeLine( std::string new_node );
		std::string createNode( int val );
		void setNode( int offset, int new_value, int pos, std::vector<std::string> node );
        void display( int offset );


};
#endif
