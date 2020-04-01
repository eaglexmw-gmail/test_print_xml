// TestPrintXML.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

// TINYXML :  1
// TINYXML2:  2
// LIBXML2 :  3
// RAPIDXML:  4
// PUGIXML :  5
// libroxml:  6
#define XML_LIBRARY  6

static void print_offset(unsigned int offset)
{
	int i = 0;
	while (i < offset) {
		putchar(' ');
		i++;
	}
}

// 这儿使用tinyxml
#if XML_LIBRARY == 1

#include <tinyxml.h>

#pragma comment(lib, "winmm.lib ")

bool _has_valid_code(const char* strBuff)
{
	const char* str = strBuff;
	while (*str != '\0')
	{
		if ((*str == '\x0d') || (*str == '\x0a') || (*str == ' ')) {
			str++;
		}
		else {
			return true;
		}
	}
	return false;
}

//解析每一个phone，提取出name、tel、address
static int print_xml_node(TiXmlDocument* doc, TiXmlNode* cur, unsigned int offset)
{
	const TiXmlAttribute* propertyNode;

	// 获取当前结点的所有属性结点，并依次打印
	print_offset(offset);
	switch (cur->Type()) {
	case TiXmlNode::NodeType::TINYXML_ELEMENT:
		printf("Node: %s, type: element\n", cur->Value());
		break;
	case TiXmlNode::NodeType::TINYXML_DOCUMENT:
		printf("Node: %s, type: document\n", cur->Value());
		break;
	case TiXmlNode::NodeType::TINYXML_COMMENT:
		printf("Node: %s, type: comment\n", cur->Value());
		break;
	case TiXmlNode::NodeType::TINYXML_TEXT:
		printf("Node: %s, type: text\n", cur->Value());
		break;
	case TiXmlNode::NodeType::TINYXML_DECLARATION:
		printf("Node: %s, type: declaration\n", cur->Value());
		break;
	case TiXmlNode::NodeType::TINYXML_TYPECOUNT:
		printf("Node: %s, type: typecount\n", cur->Value());
		break;
	case TiXmlNode::NodeType::TINYXML_UNKNOWN:
		printf("Node: %s, type: unknown\n", cur->Value());
		break;
	default:
		printf("Node: %s, bad type\n", cur->Value());
		break;
	}
	if (cur->Type() == TiXmlNode::NodeType::TINYXML_ELEMENT) {
		propertyNode = cur->ToElement()->FirstAttribute();
		while (propertyNode)
		{
			print_offset(offset + 4);
			printf("%s=[%s]\n", propertyNode->Name(), propertyNode->Value());
			propertyNode = propertyNode->Next();
		}
	}
	// 递归输出所有子结点
	TiXmlNode* childNode = cur->FirstChild();
	while (childNode != NULL) {
		print_xml_node(doc, childNode, offset + 2);
		childNode = childNode->NextSibling();
	}
	return 0;
}

static int parse_xml_file(const char* file_name)
{
	if (NULL == file_name) {
		return -1;
	}

	TiXmlDocument doc;   //xml整个文档的树形结构
	TiXmlElement* cur;  //xml节点

	//获取树形结构
	if (!doc.LoadFile(file_name)) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
	}

	//获取根节点
	cur = doc.RootElement();
	if (cur == NULL) {
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	// 依次打印每一个结点
	if (print_xml_node(&doc, cur, 0)) {
		fprintf(stderr, "iter node fail.\n");
		goto FAILED;
	}

	return 0;
FAILED:
	return -1;
}
#endif

// 这儿使用tinyxml2
#if XML_LIBRARY == 2
#include <tinyxml2.h>

using namespace tinyxml2;

bool _has_valid_code(const char* strBuff)
{
	const char* str = strBuff;
	while (*str != '\0')
	{
		if ((*str == '\x0d') || (*str == '\x0a') || (*str == ' ')) {
			str++;
		}
		else {
			return true;
		}
	}
	return false;
}

//解析每一个phone，提取出name、tel、address
static int print_xml_node(XMLDocument* doc, XMLNode* cur, unsigned int offset)
{
	const XMLAttribute* propertyNode;

	// 获取当前结点的所有属性结点，并依次打印
	print_offset(offset);
	if (cur->ToElement() != NULL) {
		printf("Node: %s, type: element\n", cur->Value());
	} else if (cur->ToText() != NULL) {
		printf("Node: %s, type: text\n", cur->Value());
	} else if (cur->ToComment() != NULL) {
		printf("Node: %s, type: comment\n", cur->Value());
	} else if (cur->ToDeclaration() != NULL) {
	    printf("Node: %s, type: declaration\n", cur->Value());
	} else if (cur->ToDocument() != NULL) {
		printf("Node: %s, type: document\n", cur->Value());
	} else if (cur->ToUnknown() != NULL) {
		printf("Node: %s, type: unknown\n", cur->Value());
	}
	if (cur->ToElement() != NULL) {
		propertyNode = cur->ToElement()->FirstAttribute();
		while (propertyNode)
		{
			print_offset(offset + 4);
			printf("%s=[%s]\n", propertyNode->Name(), propertyNode->Value());
			propertyNode = propertyNode->Next();
		}
	}
	// 递归输出所有子结点
	XMLNode* childNode = cur->FirstChild();
	while (childNode != NULL) {
		print_xml_node(doc, childNode, offset + 2);
		childNode = childNode->NextSibling();
	}
	return 0;
}

static int parse_xml_file(const char* file_name)
{
	if (NULL == file_name) {
		return -1;
	}

	XMLDocument doc;   //xml整个文档的树形结构
	XMLElement* cur;   //xml节点

	//获取树形结构
	if (XML_SUCCESS != doc.LoadFile(file_name)) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
	}

	//获取根节点
	cur = doc.RootElement();
	if (cur == NULL) {
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	// 依次打印每一个结点
	if (print_xml_node(&doc, cur, 0)) {
		fprintf(stderr, "iter node fail.\n");
		goto FAILED;
	}

	return 0;
FAILED:
	return -1;
}
#endif

// 使用libxml
#if XML_LIBRARY == 3
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlstring.h>

bool _has_valid_code(const xmlChar* strBuff)
{
	const xmlChar* str = strBuff;
	while (*str != '\0')
	{
		if ( (*str == '\x0d') || (*str == '\x0a') || (*str == ' ') ) {
			str++;
		}else {
			return true;
		}
	}
	return false;
}

//解析每一个phone，提取出name、tel、address
static int print_xml_node(xmlDocPtr doc, xmlNodePtr cur, unsigned int offset)
{
	xmlChar* key;
	xmlNodePtr childeNode;
	struct _xmlAttr* propertyNode;

	// 获取当前结点的所有属性结点，并依次打印
	if (cur->type == XML_TEXT_NODE) {
		if (cur->content[0] == '\n') {
			key = nullptr;
		}
		if (!_has_valid_code(cur->content) ) {
			print_offset(offset);
			printf("Node: %s, null content, type: %d\n", cur->name, cur->type);
		}
		else {
			print_offset(offset);
			printf("Node: %s, content: %s, type: %d\n", cur->name, cur->content, cur->type);
		}
	} else {
		print_offset(offset);
		printf("Node: %s, type: %d\n", cur->name, cur->type);
	}
	propertyNode = cur->properties;
	while (propertyNode)
	{
		print_offset(offset + 4);
		printf("%s=[%s]\n", propertyNode->name, propertyNode->children->content);
		propertyNode = propertyNode->next;
	}

	childeNode = cur->xmlChildrenNode;
	while (childeNode != NULL) {
		print_xml_node(doc, childeNode, offset + 2);
		childeNode = childeNode->next;
	}
	return 0;
}

static int parse_xml_file(const char* file_name)
{
	if (NULL == file_name) {
		return -1;
	}

	xmlDocPtr doc;   //xml整个文档的树形结构
	xmlNodePtr cur;  //xml节点
	xmlChar* id;     //phone id

	//获取树形结构
	doc = xmlParseFile(file_name);
	if (doc == NULL) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
	}

	//获取根节点
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	// 依次打印每一个结点
	if (print_xml_node(doc, cur, 0)) {
		fprintf(stderr, "iter node fail.\n");
		goto FAILED;
	}
	
	xmlFreeDoc(doc);
	return 0;
FAILED:
	if (doc) {
		xmlFreeDoc(doc);
	}
	return -1;
}
#endif

// 使用rapidxml
#if XML_LIBRARY == 4

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

#pragma comment(lib, "winmm.lib ")

using namespace rapidxml;

bool _has_valid_code(const char* strBuff)
{
	const char* str = strBuff;
	while (*str != '\0')
	{
		if ((*str == '\x0d') || (*str == '\x0a') || (*str == ' ')) {
			str++;
		}
		else {
			return true;
		}
	}
	return false;
}

//解析每一个phone，提取出name、tel、address
static int print_xml_node(xml_document<>* doc, xml_node<>* cur, unsigned int offset)
{
	const xml_attribute<>* propertyNode;

	// 获取当前结点的所有属性结点，并依次打印
	print_offset(offset);
	printf("Node: %s, value: %s, type: %d\n", cur->name(), cur->value(), cur->type());
	propertyNode = cur->first_attribute();
	while (propertyNode)
	{
		print_offset(offset + 4);
		printf("%s=[%s]\n", propertyNode->name(), propertyNode->value());
		propertyNode = propertyNode->next_attribute();
	}
	// 递归输出所有子结点
	xml_node<>* childNode = cur->first_node();
	while (childNode != NULL) {
		print_xml_node(doc, childNode, offset + 2);
		childNode = childNode->next_sibling();
	}
	return 0;
}

static int parse_xml_file(const char* file_name)
{
	if (NULL == file_name) {
		return -1;
	}

	rapidxml::file<> fdoc(file_name);
	xml_document<> doc;   //xml整个文档的树形结构
	xml_node<>* cur;  //xml节点

	//获取树形结构
	try {
		doc.parse<0>(fdoc.data());
	} catch(...) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		return -1;
	}

	//获取根节点
	cur = doc.first_node();
	if (cur == NULL) {
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	// 依次打印每一个结点
	if (print_xml_node(&doc, cur, 0)) {
		fprintf(stderr, "iter node fail.\n");
		goto FAILED;
	}

	return 0;
FAILED:
	return -1;
}
#endif

// 使用pugixml
#if XML_LIBRARY == 5

#include <pugixml.hpp>

using namespace pugi;

bool _has_valid_code(const char* strBuff)
{
	const char* str = strBuff;
	while (*str != '\0')
	{
		if ((*str == '\x0d') || (*str == '\x0a') || (*str == ' ')) {
			str++;
		}
		else {
			return true;
		}
	}
	return false;
}

//解析每一个phone，提取出name、tel、address
static int print_xml_node(xml_document& doc, xml_node& cur, unsigned int offset)
{
	xml_attribute propertyNode;

	// 获取当前结点的所有属性结点，并依次打印
	print_offset(offset);
	printf("Node: %s, value: %s, type: %d\n", cur.name(), cur.value(), cur.type());
	propertyNode = cur.first_attribute();
	while (propertyNode)
	{
		print_offset(offset + 4);
		printf("%s=[%s]\n", propertyNode.name(), propertyNode.value());
		propertyNode = propertyNode.next_attribute();
	}
	// 递归输出所有子结点
	xml_node childNode = cur.first_child();
	while (childNode != NULL) {
		print_xml_node(doc, childNode, offset + 2);
		childNode = childNode.next_sibling();
	}
	return 0;
}

static int parse_xml_file(const char* file_name)
{
	if (NULL == file_name) {
		return -1;
	}

	xml_document doc;   //xml整个文档的树形结构
	xml_node cur;  //xml节点

	//获取树形结构
	xml_parse_result result = doc.load_file(file_name);
	if (!result) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
	}

	//获取根节点
	cur = doc.first_child();
	if (cur == NULL) {
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	// 依次打印每一个结点
	if (print_xml_node(doc, cur, 0)) {
		fprintf(stderr, "iter node fail.\n");
		goto FAILED;
	}

	return 0;
FAILED:
	return -1;
}
#endif

// 使用libroxml
#if XML_LIBRARY == 6

#include "roxml.h"

#ifdef DEBUG
#pragma comment(lib, "roxml_debug.lib ")
#else
#pragma comment(lib, "roxml_release.lib ")
#endif // DEBUG

bool _has_valid_code(const char* strBuff)
{
	const char* str = strBuff;
	while (*str != '\0')
	{
		if ((*str == '\x0d') || (*str == '\x0a') || (*str == ' ')) {
			str++;
		}
		else {
			return true;
		}
	}
	return false;
}

//解析每一个phone，提取出name、tel、address
static int print_xml_node(node_t* xml, unsigned int offset)
{
	node_t* propertyNode;

	// 获取当前结点的所有属性结点，并依次打印
	print_offset(offset);
	char* name = roxml_get_name(xml, NULL, 1);
	char* value = roxml_get_content(xml, NULL, 1, NULL);
	printf("Node: %s, value: %s, type: %d\n", name, value, roxml_get_type(xml));
	int attr_index = 0;
	int attr_number = roxml_get_attr_nb(xml);
	while (attr_index < attr_number)
	{
		propertyNode = roxml_get_attr(xml, NULL, attr_index);
		print_offset(offset + 4);
		name = roxml_get_name(propertyNode, NULL, 1);
		value = roxml_get_content(propertyNode, NULL, 1, NULL);
		printf("%s=[%s]\n", name, value);
		attr_index++;
	}
	// 递归输出所有子结点
	node_t* childNode = roxml_get_chld(xml, NULL, 0);
	while (childNode != NULL) {
		print_xml_node(childNode, offset + 2);
		childNode = roxml_get_next_sibling(childNode);
	}
	return 0;
}

static int parse_xml_file(const char* file_name)
{
	if (NULL == file_name) {
		return -1;
	}

	//获取树形结构
	node_t* root = roxml_load_doc((char*)file_name);
	if (!root) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
	}

	// 依次打印每一个结点
	if (print_xml_node(root, 0)) {
		fprintf(stderr, "iter node fail.\n");
		roxml_release(RELEASE_ALL);
		roxml_close(root);
		goto FAILED;
	}

	roxml_release(RELEASE_ALL);
	roxml_close(root);

	return 0;
FAILED:
	return -1;
}
#endif

int main(int argc, char* argv[])
{
    //std::cout << "Hello World!\n";
	char* xml_file = NULL;

	if (argc >= 2) {
		xml_file = argv[1];
	}
	else {
		fprintf(stderr, "need xml file name.\n");
		return -1;
	}
    if (parse_xml_file(xml_file) != 0) {
         fprintf(stderr, "Failed to parse xml.\n");
         return -1;
	}
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
