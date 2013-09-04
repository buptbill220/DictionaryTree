#include "DictionaryTree.h"

void free_key( gpointer key );
void free_value( gpointer value );
void display_tree( gpointer key , gpointer value , gpointer user_data );

void free_key( gpointer key ){
	gchar* str = (gchar *)key;
	printf("free key:%s\n",str);
	g_free( key );
	key = NULL;
}

void free_value( gpointer value ){
	PTREENODE treenode = (PTREENODE) value;
	if( treenode->GList && treenode->ch )
		g_hash_table_destroy(treenode->GList);
	printf("free value memory:%d\n",value);
	g_free(value);
	value = NULL;
}

void display_tree( gpointer key , gpointer value , gpointer user_data ){
	//printf("key=%s,value=%u\n",key,value);
	PTREENODE treenode = (PTREENODE) value;
	gint space_count = gint( *(gint*)user_data ) + 5;
	if( treenode->GList && treenode->ch )
		g_hash_table_foreach( treenode->GList , display_tree , &space_count );
	for( int i = 0 ; i < space_count ; ++i )
		printf(" ");
	printf( "%c\n" , treenode->ch );
}

DictionaryTree::DictionaryTree(){
	m_nWordCount = 0;
	rootTable = g_hash_table_new_full( g_str_hash , g_str_equal , free_key , free_value );
}

DictionaryTree::~DictionaryTree(){
	g_hash_table_destroy(rootTable);
}

int DictionaryTree::InsertNode( PTREENODE ptreeNode , const gchar* str ){
	gchar *temp = (gchar *)g_malloc0( sizeof(gchar) * 2 );
	temp[0] = *str , temp[1] = '\0';
	gpointer nextNode = NULL;
	if( ptreeNode->GList && *str ){
		nextNode = g_hash_table_lookup( ptreeNode->GList ,temp );
		if( nextNode ){
			g_free( temp );
			return InsertNode( (PTREENODE)nextNode , &str[1] ) + 1;
		}
		else{
			PTREENODE treenode = (PTREENODE)g_malloc0(sizeof(TREENODE));
			treenode->ch = *str;
			treenode->GList = g_hash_table_new_full( g_str_hash, g_str_equal , free_key , free_value );
			g_hash_table_insert( ptreeNode->GList , temp , treenode );
			return InsertNode( treenode , &str[1] ) + 1;
		}
	}else if( ptreeNode->GList ){
		temp[0] = '#';
		nextNode = g_hash_table_lookup( ptreeNode->GList ,"#" );
		if( nextNode ){
			g_free( temp );
			return 0;
		}
		else{
			PTREENODE treenode = (PTREENODE)g_malloc0(sizeof(TREENODE));
			treenode->ch = '#';
			g_hash_table_insert( ptreeNode->GList , temp , treenode );
			return 0;
		}
	}else{
		g_free( temp );
		return 0;
	}
}

int DictionaryTree::InsertNode( const gchar* str ){
	if( *str ){
		++m_nWordCount;
		gchar *temp = (gchar *)g_malloc0( sizeof(gchar) * 2 );
		temp[0] = *str , temp[1] = '\0';
		gpointer nextNode =  g_hash_table_lookup( rootTable ,temp );
		if( nextNode ){
			g_free( temp );
			return InsertNode( (PTREENODE)nextNode , &str[1] ) + 1;
		}
		else{
			PTREENODE treenode = (PTREENODE)g_malloc0(sizeof(TREENODE));
			treenode->ch = *str;
			treenode->GList = g_hash_table_new_full( g_str_hash, g_str_equal , free_key , free_value );
			g_hash_table_insert( rootTable , temp , treenode );
			return InsertNode( treenode , &str[1] ) + 1;
		}
	}
	return 0;
}

int DictionaryTree::SearchKey( const gchar* key ){
	if( *key ){
		gchar temp[2];
		temp[0] = *key , temp[1] = '\0';
		gpointer nextNode =  g_hash_table_lookup( rootTable ,temp );
		if( nextNode )
			return SearchKey( (PTREENODE)nextNode , &key[1] ) + 1;
		else
			return 100;
	}
	return 100;
}

int DictionaryTree::SearchKey( PTREENODE ptreeNode , const gchar* key ){
	gchar temp[2];
	temp[0] = *key , temp[1] = '\0';
	gpointer nextNode = NULL;
	if( ptreeNode->GList && *key ){
		nextNode = g_hash_table_lookup( ptreeNode->GList ,temp );
		if( nextNode )
			return SearchKey( (PTREENODE)nextNode , &key[1] ) + 1;
		else if( g_hash_table_lookup( ptreeNode->GList , "#" ) )
			return 300;
		else
			return 100;
	}else if( ptreeNode->GList ){
		if( g_hash_table_lookup( ptreeNode->GList , "#" ) )
			return 0;
		else
			return 200;
	}else
		return 100;
}

void DictionaryTree::Display_Hash_Table(){
	gint space_count = 0;
	//g_hash_table_foreach( rootTable , print_key_value , NULL );
	g_hash_table_foreach( rootTable , display_tree , &space_count );
}

int DictionaryTree::GetJudge( int code , int &match , int &like ){
	match = ( code >= 100 ) ? UNMATCH : MATCH;
	like = code / 100;
	return ( code % 100 );
}