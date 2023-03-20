#include "pagechange.h"
page_struct::page_struct(){
    pagecontrol = -1;
    inmemory = 0;
    used_information = 0;
}
pagecontrol_struct::pagecontrol_struct(){
    page = -1;
    used = 0;
} 