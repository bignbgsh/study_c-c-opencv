#ifndef CONTACT_MANAGER_H
#define CONTACT_MANAGER_H

#include <string>
#include <vector>

struct contact{
   std::string name;
   std::string telephone;
   std::string qq_or_email;
};


void inputcontacts (std::vector<contact> &contacts);

void printcontacts(std::vector<contact>& contacts);

void findcontacts(const std::vector<contact>& contacts);

void erasecontacts(std::vector<contact>& contacts);

bool savecontacts(const std::vector<contact>& contacts);

void loadcontacts(std::vector<contact>& contacts);












#endif