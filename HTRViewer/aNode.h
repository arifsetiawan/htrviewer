
/************************************************************************
 * Amy Project - part of vision based human motion capture project
 * 
 * Nurul Arif Setiawan
 * Intelligent Image Media Laboratory
 * Dept. of Computer and Engineering
 * Chonnam National University, Gwangju, South Korea
 *
 * aNode.h
 * Struct to represent human body skeleton and other structures 
 * 
 * History : - May 19 2005 - Initial work is based on M. Meredith and S. Maddock paper 
 *							 "Motion Capture File Format Explained"
 ************************************************************************/

#ifndef _AMY_NODE_H_
#define _AMY_NODE_H_


/************************************************************************
 * Basic stuctures for Node
 ************************************************************************/

/**
 * Not implemented
 */
struct OBJECTINFO 
{
};


/**
 * Not implemented
 */
struct CONSTRAINT 
{
};

/**
 * Node - represent a bone in articulated body 
 */
struct NODE
{
	char *name;
	float length;			// length of segment along y-axis
	float offset[3];		// transitional offset with respect to the end of parent link
	float euler[3];			// rotation of base position
	float colour[3];		// colour used when displaying wire frame skeleton
	int noofchildren;		// number of children nodes
	NODE **children;		// array of pointer to children node
	NODE *parent;			// back pointer to parent node
	float **froffset;		// array offset for each frame
	float **freuler;		// array of angles for each frame
	float *frscale;			// array of scale factor for each frame
	short DOFs;				// what DOFs the segment has
	OBJECTINFO *object;		// point to 3D object that is the limb
	CONSTRAINT *constraint;	// list of constraint - ordered in time
};


/**
 * HTR file header
 */
struct MOCAPHEADER 
{
	int noofsegments;		// number of body segments
	long noofframes;		// number of frames
	int datarate;			// frame per second
	int euler[3][3];		// how euler angle is defined
	float callib;			// scale factor for converting current translation units into meters
	bool degrees;			// are rotational measurements in degrees
	float scalefactor;		// global scale factor
	long currentframe;		// store current frame to render
	float floor;			// position of floor along y axis
};


/**
 * General wrapper for HTR parser 
 */
struct MOCAPSEGMENT 
{
	char *name;				// name of motion capture file
	NODE *root;				// root node pointer of the animation
	MOCAPHEADER *header;	// pointer to structure contain parameters 
	NODE **nodelist;		// Array of pointers to skeletal nodes
};



/************************************************************************
 * Others
 ************************************************************************/

/**
 * HTR file's part enumeration 
 */
typedef enum
{
	HEADER,
	SEGMENTS,
	BASE,
	MOTION
} SECTION;



#endif