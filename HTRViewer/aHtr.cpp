
/************************************************************************
 * aHtr.cpp
 ************************************************************************/

// this is vs6.0 bug!!
#pragma warning(disable:4786)

#include <math.h>
#include "aHtr.h"
#include "aString.h"
#include <gl\glut.h>


/**
 * Constructor
 */
aHTRFormat::aHTRFormat()
{
	header = (MOCAPHEADER*)malloc(sizeof(struct MOCAPHEADER));
	root =(NODE*) malloc(sizeof(NODE));
}

/**
 * Destructor
 */
aHTRFormat::~aHTRFormat()
{
	free(header);
	free(root);
}

/**
 * Import data from a file
 */
bool aHTRFormat::ImportData(const char *filename)
{
	printf("reading htr file.........\n");
	
	DelimitedFileParser fileParser(filename, "\t ");
	fileParser.Parse();

	string str_col0, str_col1, str_temp;
	int section;
	int currentnode = 0;
	int i, j, k, jj = 0; // counters
	NODE *parent, *child, **p_temp, *tnode;

	printf("%d Rows found\n", fileParser.NumRows() );

	// process every row
	for ( i = 0; i < fileParser.NumRows() ; ++i )
	{
		// this is row to be processed
		DelimitedRow row = fileParser.getRow(i);
		row.DelEmpty();
		
	//	printf("Row: %d\n", i );

		str_col0 = row.getColumn(0);
		deleteEmpty(str_col0);

		// process a ptr_col only if first character is not "#"
		// this row is not comment
		if (str_col0[0] != '#')
		{
			
			// find header
			if ( !nocase_cmp(str_col0, "[HEADER]"))
			{
				printf("---Found Header\n");
				section = HEADER;
			}
			// find segment names and hierarchy
			else if (!nocase_cmp(str_col0, "[SEGMENTNAMES&HIERARCHY]"))
			{
				printf("---Found SegmentNames&Hierarchy\n");
				section = SEGMENTS;
			}
			// find base position
			else if (!nocase_cmp(str_col0, "[BASEPOSITION]"))
			{
				printf("\n---Found BasePosition\n");
				printf("number of nodes : %d\n", currentnode);


				// body sructure has been read
				// assign global child to root before we process base position
				root = 0;

				for ( j = 0; j < currentnode && !root; ++j )
				{
					if (!nocase_cmp(nodelist[j]->name, "GLOBAL"))
					{
						root = nodelist[j]->children[0];
						root->parent = 0;
						
						p_temp = (NODE**)malloc(sizeof(NODE*)*header->noofsegments);
						int m;
						for ( m = 0; m < j; ++m )
							p_temp[m] = nodelist[m];
						for ( m = j+1; m <= header->noofsegments ; ++m )
							p_temp[m-1] = nodelist[m];

						
						for (m = 0; m < header->noofframes; ++m)
						{
							free(nodelist[j]->froffset[m]);
							free(nodelist[j]->freuler[m]);
						}
						free(nodelist[j]->froffset);
						free(nodelist[j]->freuler);
						free(nodelist[j]->frscale);
						free(nodelist[j]->name);
						free(nodelist[j]->children);
						free(nodelist[j]);
						free(nodelist);
						nodelist = p_temp;
						--currentnode;
						
					}
				}

				printf("Root name %s \n", root->name);
				printf("Root noofchild %d \n", root->noofchildren);
				printf("number of nodes after deleting global node: %d\n", currentnode);
				printf("List of nodes from %d nodes\n", currentnode);
				for (j = 0; j < currentnode; ++j )
				{
					DumpNode(nodelist[j],j+1);
				}

			//	k = 0;

				section = BASE;	
			}
			// find animation data;
			else if (str_col0[0] == '[')
			{
				section = MOTION;
				// break;
			}
			// end of file 
			if (!nocase_cmp(str_col0, "[ENDOFFILE]"))
			{
				printf("Reach end of file \n");
				return true;
				break;
			}

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// process header
			if (section == HEADER)
			{	
				str_col1 = row.getColumn(1);
				deleteEmpty(str_col1);
				
				// check filetype
				if (!nocase_cmp(str_col0, "FILETYPE"))
				{	
					if (str_col1  != "htr")
						printf("------This is not htr file, what are you doing!\n");
					else
						printf("------This is htr file\n");
				
				} 
				// check datatype
				else if(!nocase_cmp(str_col0, "DATATYPE"))
				{
					if (str_col1  != "HTRS")
						printf("------Incompatible composition!\n");
					else
						printf("------HTRS!\n");
				}
				// check file version
				else if (!nocase_cmp(str_col0, "FILEVERSION"))
				{
					if (atoi(str_col1.c_str() ) != 1)
						printf("File not recognized!");
					else
						printf("------File version got!\n");
				}
				// number of segments
				else if (!nocase_cmp(str_col0, "NUMSEGMENTS"))
				{
					header->noofsegments = atoi(str_col1.c_str());
					printf("------Segment number %d!\n", header->noofsegments);
					nodelist = (NODE**)malloc(sizeof(NODE*)*(header->noofsegments + 1));
				}
				// number of frames
				else if (!nocase_cmp(str_col0, "NUMFRAMES"))
				{
					header->noofframes = atoi(str_col1.c_str());
					printf("------Frame number %d!\n", header->noofframes);
				}
				// frame rate
				else if (!nocase_cmp(str_col0, "DATAFRAMERATE"))
				{
					header->datarate = atoi(str_col1.c_str());
					printf("------Data rate  %d!\n", header->datarate);
				}
				// rotation order
				else if (!nocase_cmp(str_col0, "EULERROTATIONORDER"))
				{
					// setup euler orientation angle
					// TODO : still don't get this part
					for (j = 0; j < 3; j++)
					{
						switch(str_col1.c_str()[j])
						{
						case 'Z':
							zpos = j;
							break;
						case 'Y':
							ypos = j;
							break;
						case 'X':
							xpos = j;
						}
					}
					printf("------Rotation Order  %s  -%d-%d-%d!\n", str_col1.c_str(), zpos, ypos, xpos);

					header->euler[0][0] = header->euler[0][1] = 0;
					header->euler[0][2] = 1;
					header->euler[1][0] = header->euler[1][2] = 0;
					header->euler[1][1] = 1;
					header->euler[2][1] = header->euler[2][2] = 0;
					header->euler[2][0] = 1;

				}
				// calibration units
				else if (!nocase_cmp(str_col0, "CALIBRATIONUNITS"))
				{
					if (!nocase_cmp(str_col1, "MM"))
						header->callib = 1.0f/1000.0f;
					else if (!nocase_cmp(str_col1, "DM"))
						header->callib = 1.0f/10.0f;
					else if (!nocase_cmp(str_col1, "CM"))
						header->callib = 1.0f/100.0f;
					else if (!nocase_cmp(str_col1, "INCHES"))
						header->callib = 1.0f/40.0f;
					else
						header->callib = 1.0f;

					printf("------Calibration Unit %s - %f!\n", str_col1.c_str(), header->callib);
				}
				// rotation unit
				else if (!nocase_cmp(str_col0, "ROTATIONUNITS"))
				{
					if (!nocase_cmp(str_col1, "DEGREES"))
						header->degrees = true;
					else 
						header->degrees = false;

					printf("------Rotation unit =  %s!\n", str_col1.c_str());
				}
				// global scale factor
				else if (!nocase_cmp(str_col0, "SCALEFACTOR"))
				{
					header->scalefactor = (float) atof(str_col1.c_str());
					printf("------Scale factor =  %f!\n", header->scalefactor);
				}
			} // end of section HEADER
		

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// process segments
			// this is where skeleton construction intializes
			if (section == SEGMENTS)
			{
				str_col1 = row.getColumn(1);
				deleteEmpty(str_col1);

				// if second column is not zero then we know it is child - parent 
				if(str_col1.size() != 0)
				{
					parent = 0; child = 0;
					
					printf("\n++++++++++++++++++++++++++++++++++\n");
					printf("Pair to process : %s - %s \n", str_col0.c_str(), str_col1.c_str());
					printf("current node value : %d \n", currentnode);

					for ( k = 0; k < currentnode; ++k )
					{
						str_temp = nodelist[k]->name;
						deleteEmpty(str_temp);
			
						if(!nocase_cmp(str_col0, str_temp))
							child = nodelist[k];
	
						if(!nocase_cmp(str_col1, str_temp))
							parent = nodelist[k];
					}
					
					// check if we have cyclic child-parent relationship
					if ( child && child == parent )
					{
						printf("Cyclic child-parent relationship\n");
						return false;
					}

					// if child is node defined
					if (!child)
					{
						printf("child node node has't been defined, run this!!\n");
						nodelist[currentnode] = (NODE*) malloc(sizeof(NODE));
						nodelist[currentnode]->name = (char*)malloc(sizeof(char)*(strlen(str_col0.c_str())+1));
						nodelist[currentnode]->object = 0;
						nodelist[currentnode]->constraint = 0;
						strcpy(nodelist[currentnode]->name, str_col0.c_str());
						SetupChildren(nodelist[currentnode], 0);
						SetupFrame(nodelist[currentnode], header->noofframes);
					//	child = nodelist[0];
						child = nodelist[currentnode++];
						DumpNode(nodelist[currentnode-1], currentnode-1);
					}
					
					// if parent is node defined
					if (!parent)
					{
						printf("parent node node has't been defined, run this!!\n");
						nodelist[currentnode] = (NODE*) malloc(sizeof(NODE));
						nodelist[currentnode]->name = (char*)malloc(sizeof(char)*(strlen(str_col1.c_str())+1));
						nodelist[currentnode]->object = 0;
						nodelist[currentnode]->constraint = 0;
						strcpy(nodelist[currentnode]->name, str_col1.c_str());
						SetupChildren(nodelist[currentnode], 0);
						SetupFrame(nodelist[currentnode], header->noofframes);
						parent = nodelist[currentnode++];
						DumpNode(nodelist[currentnode-1],currentnode-1);
					}
					child->parent = parent;
					
					if (parent->children)
					{
						printf("parent %s already has children\n", parent->name);
						printf("number of children was %d\n", parent->noofchildren);
						// parent already has children
						p_temp = (NODE**)malloc(sizeof(NODE*)*parent->noofchildren);
						for ( j = 0; j < parent->noofchildren; ++j )
							p_temp[j] = parent->children[j];

						free(parent->children);
	
						parent->children = (NODE**)malloc(sizeof(NODE*)* ++parent->noofchildren);
						for ( j = 0; j < parent->noofchildren; ++j )
							parent->children[j] = p_temp[j];

						free(p_temp);
						parent->children[parent->noofchildren-1] = child;
						printf("number of children now %d\n", parent->noofchildren);
					}
					else
					{
						printf("parent %s doesn't have children\n", parent->name);
						SetupChildren(parent, ++parent->noofchildren);
						parent->children[0] = child;
						printf("parent %s now has children %s\n", parent->name, child->name);
					}

					if (currentnode > header->noofsegments + 1)
					{
						printf("There are mode segments than specified in header!\n");
						return false;
					}
				}

			} // end of section SEGMENT

			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// process base offset, orientation and scale
			if (section == BASE)
			{
				if(nocase_cmp(str_col0, "[BASEPOSITION]"))
				{
				//	printf("processing base for node %s\n", row.getColumn(0).c_str());

					tnode = 0;

					for (j = 0; j < currentnode && !tnode; ++j)
						if (!strcmp(nodelist[j]->name, str_col0.c_str()))
							tnode = nodelist[j];

					// do setup
					SetupColour(tnode);
					SetupOffset(tnode,	(float) atof(row.getColumn(1).c_str())*header->callib, 
										(float) atof(row.getColumn(2).c_str())*header->callib,
										(float) atof(row.getColumn(3).c_str())*header->callib);
					
					tnode->euler[xpos] = (float) atof(row.getColumn(4).c_str());
					tnode->euler[ypos] = (float) atof(row.getColumn(5).c_str());
					tnode->euler[zpos] = (float) atof(row.getColumn(6).c_str());
					
					if (!header->degrees)
					{
						tnode->euler[xpos] *= 57.2957795f;
						tnode->euler[ypos] *= 57.2957795f;
						tnode->euler[zpos] *= 57.2957795f;
					}
					
					tnode->length = (float)atof(row.getColumn(7).c_str()) * header->callib;
					
					tnode->DOFs = 6;

				}

			} // end of section BASE

			
			// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			// process base offset, orientation and scale
			if (section == MOTION)
			{
				
				if (str_col0[0] == '[')
				{	
					tnode = 0;
					
					printf("---processing data for node %s\n", row.getColumn(0).c_str());
				
					for (j = 0; j < currentnode && !tnode; ++j)
					{
						str_temp = nodelist[j]->name;
						str_temp = "["+str_temp+"]";

						if (!strcmp(str_temp.c_str(), str_col0.c_str()))
							tnode = nodelist[j];
					}
				//	printf("---This is animation data for  %s\n", tnode->name);
				}
				else
				{
				//	printf("Data for %s is taken from this line each frame here!\n", tnode->name);

				//	printout to check!
				//	for ( int j=0; j<row.NumColumns(); ++j )			
				//		printf("Column %d = [%s]\n", j, row.getColumn(j).c_str() );

					int frame = atoi(row.getColumn(0).c_str())-1;
					
					tnode->freuler[frame][0] = (float) atof(row.getColumn(6).c_str());
					tnode->freuler[frame][1] = (float) atof(row.getColumn(5).c_str());
					tnode->freuler[frame][2] = (float) atof(row.getColumn(4).c_str());
					

					// offset
					tnode->froffset[frame][0] = (float) atof(row.getColumn(1).c_str())*header->callib;
					tnode->froffset[frame][1] = (float) atof(row.getColumn(2).c_str())*header->callib;
					tnode->froffset[frame][2] = (float) atof(row.getColumn(3).c_str())*header->callib;

					// scale
					tnode->frscale[frame] = (float) atof(row.getColumn(7).c_str());
					
				}

			} // end of section BASE

		} // end for every not-comment row

	} // end for every row


	return true;
}

/**
 * Allocate memory for children of a node
 */
void aHTRFormat::SetupChildren(NODE *node, int i)
{
	node->noofchildren = i;
	
	if(i)
		node->children = (NODE**)malloc(sizeof(NODE*)* i);
	else
		node->children = NULL;
}

/**
 * allocate memory for animation data for a node
 */
void aHTRFormat::SetupFrame(NODE *node, int i)
{
	if ( i )
	{
		int j;
		
		node->froffset = (float**)malloc(sizeof(float*)*i);
			for(j = 0; j < i; j++)
				node->froffset[j] = (float*)malloc(sizeof(float[3]));
		
		node->freuler = (float**)malloc(sizeof(float*)*i);
			for(j = 0; j < i; j++)
				node->freuler[j] = (float*)malloc(sizeof(float[3]));
		
		node->frscale = (float*)malloc(sizeof(float*)*i);
	}
	else
	{
		node->froffset = 0;
		node->freuler = 0;
		node->frscale = 0;
	}
}

/**
 * Node Color
 */
void aHTRFormat::SetupColour(NODE *node)
{
	node->colour[0] = 0.0f;
	node->colour[1] = 0.0f;
	node->colour[2] = 1.0f;
}

/**
 * Setup offset for a node
 */
void aHTRFormat::SetupOffset(NODE *node, float tx, float ty, float tz)
{
	node->offset[0] = tx;
	node->offset[1] = ty;
	node->offset[2] = tz;
}

/**
 * Dump node, for debugging
 */
void aHTRFormat::DumpNode(NODE *node, int i)
{
	printf("==DUMPED====%%^_^%%=========\n");
	printf("Node %d name : %s\n", i, node->name);
	printf("Node number of child %d \n", node->noofchildren);
}


/**
 * Draw nodes without animation data
 */
void aHTRFormat::DrawBasePoseNode(NODE *node)
{
	glPushMatrix();

//	printf("Drawing node : %s\n", node->name);
//	printf("Node noofchild : %d\n", node->noofchildren);

	glTranslatef(node->offset[0], 
				 node->offset[1],
				 node->offset[2]);

	glRotatef(node->euler[0], 0.0 , 0.0, 1.0); 
	glRotatef(node->euler[1], 0.0 , 1.0, 0.0); 
	glRotatef(node->euler[2], 1.0 , 0.0, 0.0); 

	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(0.02);

	glBegin(GL_LINES);
		glColor3f(node->colour[0], node->colour[1], node->colour[2] );
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, node->length, 0.0f);
	glEnd();

	if ( node->noofchildren )
		for ( int i = 0; i < node->noofchildren; i++ )
			DrawBasePoseNode(node->children[i]);

	glPopMatrix();
}

void aHTRFormat::DrawBase()
{
	DrawBasePoseNode(root);
}

/**
 * Draw node with animation data
 */
void aHTRFormat::DrawFrameInsider(NODE *node, int currentframe)
{
	glPushMatrix();

	glTranslatef(node->offset[0] + node->froffset[currentframe][0], 
				 node->offset[1] + node->froffset[currentframe][1],
				 node->offset[2] + node->froffset[currentframe][2]);


	glRotatef(node->euler[0], 0.0 , 0.0, 1.0); 
	glRotatef(node->euler[1], 0.0 , 1.0, 0.0); 
	glRotatef(node->euler[2], 1.0 , 0.0, 0.0); 

	glRotatef(node->freuler[currentframe][0], 0.0 , 0.0, 1.0); 
	glRotatef(node->freuler[currentframe][1], 0.0 , 1.0, 0.0); 
	glRotatef(node->freuler[currentframe][2], 1.0 , 0.0, 0.0); 

	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(0.02);

	glBegin(GL_LINES);
		glColor3f(node->colour[0], node->colour[1], node->colour[2] );
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, node->length*node->frscale[currentframe], 0.0f);
	glEnd();

	if ( node->children)
		for ( int i = 0; i < node->noofchildren; i++ )
			DrawFrameInsider(node->children[i], currentframe);

	glPopMatrix();
}

void aHTRFormat::DrawFrame(int currentframe)
{
	DrawFrameInsider(root, currentframe);
}

