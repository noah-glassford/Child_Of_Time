#pragma once
#include <Box2D/Box2D.h>

//New file that contains contact listener stuff, might remove later
class MyContactListener : public b2ContactListener
{
public:
	int numFootContacts;
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
			
		
};
