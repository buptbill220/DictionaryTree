//////////////////////////////////////////////////////////////////////////  
///	COPYRIGHT NOTICE 
///	Copyright (c) 2011, Fangming
///	All rights reserved.
///	
///	@file	DictionaryTree.h
///	@brief	define dictionary tree and build it
///
///	
///	
///
///	@version	1.1
///	@author	Fangming
///	@date		2011.1.5					
///			 
///
///
/// @notes	read words from library file. 
///
///
///			 
//version 1.1
///
///
///
//////////////////////////////////////////////////////////////////////////

#ifndef _DICTIONARY_TREE_H_
#define _DICTIONARY_TREE_H_
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib/gprintf.h>

///word match in dictionary or not
#define MATCH		1
#define UNMATCH		0

//////////////////////////////////////////////////////////////////////////
///extent similarity, word matched in dictionary
///
///only parts of the sequence can be matched in dictionary, for example,
///word "glib", the library has not the same word but has "glory", then
///only "gl" can be matched in dictionary.
#define SOMELIKE	1
///the sequence can be matched in dictionary but just parts of its standard word, for example,
///word "glib", the library has not the same word but has "glibly"
#define SHORTLIKE	2
///the prefix of the sequence can be matched in dictionary, for example,
///word "glib", the library has not the same word but has "gli"
#define LONGLIKE	3
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
////@brief	the each tree node of dictionary
////@notes	the node use a character to store current character and hash table
///			to reflect subsequent node using glib library. for inner node, its
///			character store the current character, but use '#' instead for leaf node.
//////////////////////////////////////////////////////////////////////////
typedef struct TREENODE{
	///hash table to reflect subsequent node using glib library
	GHashTable* GList;
	///current node character
	gchar ch;
	///constructor
	TREENODE( gchar ch ){
		GList = NULL;
		this->ch = ch;
	}
	///overloaded operator assignment
	TREENODE& operator= ( TREENODE& treenode ){
		GList = treenode.GList;
		ch = treenode.ch;
	}
}TREENODE;

typedef TREENODE* PTREENODE;

//////////////////////////////////////////////////////////////////////////
///	@function	build dictionary from standard library file
///
///		
///
//////////////////////////////////////////////////////////////////////////
class DictionaryTree{
private:
	///dictionary root
	GHashTable *rootTable;
	///total number of words dictionary contains
	int m_nWordCount;
public:
	///    constructor
	///
	///			constructor
	///			@param none
	///			@return none
	///
	///			@notes	initial dictionary tree root and words number default 0
	///
	DictionaryTree();
	///    destructor
	///
	///			destructor
	///			@param none
	///			@return none
	///
	///			@notes	destroy all tree and free the allocated memory
	///
	~DictionaryTree();
	///    insert a word into the tree
	///
	///			@param const gchar* str	input const word
	///			@return int				the returned value reflecting the inserted depth
	///
	///			@notes
	///
	int InsertNode( const gchar* str );
	///    insert a word into the tree, use recursion algorithm
	///
	///			@param PTREENODE ptreeNode	tree node
	///			@param const gchar* str		input const word
	///			@return int					the returned value reflecting the inserted depth
	///
	///			@notes	call the other Insert Node function to complete its mission
	///
	int InsertNode( PTREENODE ptreeNode , const gchar* str );
	///    search a key word on the tree
	///
	///			@param const gchar* str	input const key word
	///			@return int				the returned code value reflecting the matching state
	///
	///			@notes	call the other Search Key function to complete its mission
	///
	int SearchKey( const gchar* key );
	///    search a key word on the tree, use recursion algorithm
	///
	///			@param PTREENODE ptreeNode	tree node
	///			@param const gchar* str		input const word
	///			@return int					the returned value reflecting the matching state
	///
	///			@notes	it has 4 states when finish. detail description are shown by GetJudge function.
	///
	int SearchKey( PTREENODE ptreeNode , const gchar* str );
	///    analysis the search code and give its state
	///
	///			@param int code		search code
	///			@param int &match	its value is MATCH if 1, else UNMATCH
	///			@param int like		extent similarity, it has 3 different values
	///			@return int			the max match length
	///
	///			@notes	if the code is between 1 and 99, then match is MATCH, else UNMATCH.
	///					if the code is between 100 and 199, then like value is SOMELIKE, else 
	///					if the code is between 200 and 299, then like value is SHORTLIKE, else
	///					if the code is no less than 300, then like value is LONGLIKE.
	///
	static int GetJudge( int code , int &match , int &like );
	///    return the total words number dictionary contains
	///
	///			@param none
	///			@return int	total words
	///
	///			@notes	before called, the tree must exist.
	///
	int WordCount() const { return m_nWordCount; }
	///    display tree
	///
	///			@param none
	///			@return node
	///
	///			@notes
	///
	void Display_Hash_Table();
};

#endif