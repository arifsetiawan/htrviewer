
/************************************************************************
 * Amy Project - part of vision based human motion capture project
 * 
 * Nurul Arif Setiawan
 * Intelligent Image Media Laboratory
 * Dept. of Computer and Engineering
 * Chonnam National University, Gwangju, South Korea
 *
 * aHtr.h
 * HTR file parser
 * 
 * History : - May 19 2005 - Initial work is based on M. Meredith and S. Maddock paper 
 *							 "Motion Capture File Format Explained"
 *							 Add some parts to make it functional
 *							 Use delimiter file read class to parse line in HTR file
 ************************************************************************/

// this is vs6.0 bug!!
#pragma warning(disable:4786)

#ifndef _AMY_HTR_H_
#define _AMY_HTR_H_

#include "aNode.h"
#include "aDelim.h"

class aHTRFormat
{
public:
	aHTRFormat();
	~aHTRFormat();
	
	bool ImportData(const char *filename);

	void SetupChildren(NODE *node, int i);
	void SetupFrame(NODE *node, int i);
	void SetupColour(NODE *node);
	void SetupOffset(NODE *node, float tx, float ty, float tz);

	void DumpNode(NODE *node, int i);
	void DumpNodes();

	void DrawBasePoseNode(NODE *node);
	void DrawBase();
	void DrawFrameInsider(NODE *node, int currentframe);
	void DrawFrame(int currentframe);

	int NumFrames() { return header->noofframes; }
	int NumFPS() { return header->datarate; }

protected:

private:
	MOCAPHEADER *header;
	NODE **nodelist;
	NODE *root;
	int zpos, ypos, xpos;
};

#endif