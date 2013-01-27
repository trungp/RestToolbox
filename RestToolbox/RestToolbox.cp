/*
 *  RestToolbox.cp
 *  RestToolbox
 *
 *  Created by Matt Jarjoura on 1/24/13.
 *
 *
 */

#include <iostream>
#include "RestToolbox.h"
#include "RestToolboxPriv.h"

void RestToolbox::HelloWorld(const char * s)
{
	 RestToolboxPriv *theObj = new RestToolboxPriv;
	 theObj->HelloWorldPriv(s);
	 delete theObj;
};

void RestToolboxPriv::HelloWorldPriv(const char * s) 
{
	std::cout << s << std::endl;
};

