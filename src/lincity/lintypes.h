/* ---------------------------------------------------------------------- *
 * lintypes.h
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * ---------------------------------------------------------------------- */
#ifndef __lintypes_h__
#define __lintypes_h__

#include <SDL_mixer.h>                  // for Mix_Chunk
#include <SDL_surface.h>                // for SDL_Surface
#include <zlib.h>                       // for gzFile
#include <algorithm>                    // for max
#include <array>                        // for array
#include <cstring>                      // for NULL
#include <iostream>                     // for basic_ostream, operator<<, cout
#include <list>                         // for list
#include <map>                          // for map
#include <string>                       // for char_traits, basic_string
#include <vector>                       // for vector

#include "engglobs.h"
#include "gui/Texture.hpp"              // for Texture

class Blacksmith;
class Coal_power;
class Coalmine;
class Commune;
class Construction;
class ConstructionGroup;
class Cricket;
class Fire;
class FireStation;
class HealthCentre;
class IndustryHeavy;
class IndustryLight;
class Market;
class Mill;
class Monument;
class Oremine;
class Organic_farm;
class Parkland;
class Port;
class Pottery;
class Powerline;
class Rail;
class RailBridge;
class Recycle;
class Residence;
class ResourceGroup;
class Road;
class RoadBridge;
class RocketPad;
class School;
class Shanty;
class SolarPower;
class Substation;
class Tip;
class Track;
class TrackBridge;
class Transport;
class University;
class Waterwell;
class Windmill;
class Windpower;
template <typename MemberType> class MemberTraits;

#define WORLD_SIDE_LEN 100
#define OLD_MAX_NUMOF_SUBSTATIONS 100
#define MAX_NUMOF_SUBSTATIONS 512

#define NUMOF_COAL_RESERVES ((world.len() * world.len()) / 400)
#define NUM_OF_TYPES    404
#define NUM_OF_GROUPS    51

#define OLD_MAX_NUMOF_MARKETS 100
#define MAX_NUMOF_MARKETS 512

#define NUMOF_DAYS_IN_MONTH 100
#define NUMOF_DAYS_IN_YEAR (NUMOF_DAYS_IN_MONTH*12)

#define NUMOF_DISCOUNT_TRIGGERS 6

//pages for report 0,1,2,3,...,LAST_REPORT_PAGE
#define LAST_REPORT_PAGE 3

/*

int get_group_cost(short group);
int get_type_cost(short type);
void get_type_name(short type, char *s);
*/
unsigned short get_group_of_type(unsigned short selected_type);
void set_map_groups(void);

struct ExtraFrame{
    ExtraFrame(void){
        move_x = 0;
        move_y = 0;
        frame = 0;
        resourceGroup = 0;
    }

    int move_x; // >0 moves frame to the right
    int move_y; // >0 moves frame downwards
    int frame; //frame >= 0 will be rendered as overlay
    ResourceGroup *resourceGroup; //overlay frame is choosen from its GraphicsInfoVector
};

// Class to count instanced objects of each construction type

template <typename Class>
class Counted
{
public:
    Counted()
    {
        instanceCount++;
        nextId++;
    }
    ~Counted()
    {
        --instanceCount;
        //reset unique only Id after the last Construction is gone
        if (instanceCount == 0)
        { nextId = 0;}
    }
    static unsigned int getInstanceCount() {
        return instanceCount;
    }
    static unsigned int getNextId() {
        return nextId;
    }
private:
    static unsigned int instanceCount, nextId;
};


template <typename Class>
unsigned int Counted<Class>::instanceCount;
template <typename Class>
unsigned int Counted<Class>::nextId;

class Ground
{
public:
    Ground();
    ~Ground();
    int altitude;       //surface of ground. unused currently
    int ecotable;       //done at init time: pointer to the table for vegetation
    int wastes;         //wastes underground
    int pollution;      //pollution underground
    int water_alt;      //altitude of water (needed to know drainage basin)
    int water_pol;      //pollution of water
    int water_wast;     //wastes in water
    int water_next;     //next tile(s) where the water will go from here
    int int1;           //reserved for future (?) use
    int int2;
    int int3;
    int int4;
};

class MapTile {
public:
    MapTile();
    ~MapTile();
    Ground ground;                        //the Ground associated to an instance of MapTile
    Construction *construction;           //the actual construction (e.g. for simulation)
    Construction *reportingConstruction;  //the construction covering the tile
    unsigned short type;                  //type of terrain (underneath constructions)
    unsigned short group;                 //group of the terrain (underneath constructions)
    int flags;                            //flags are defined in lin-city.h
    unsigned short coal_reserve;          //underground coal
    unsigned short ore_reserve;           //underground ore
    int pollution;                        //air pollution (under ground pollution is in ground[][])
    std::list<ExtraFrame> *framesptr;    //Overlays to be rendered on top of type, mostly NULL
                                          //use memberfunctions to add and remove sprites

    void setTerrain(unsigned short group); //places type & group at MapTile
    std::list<ExtraFrame>::iterator createframe(); //creates new empty ExtraFrames
                                                    //to be used by Contstructions and Vehicles
    void killframe(std::list<ExtraFrame>::iterator it); //kills an extraframe

    unsigned short getType();          //type of bare land or the covering construction
    unsigned short getTopType();       //type of bare land or the actual construction
    unsigned short getLowerstVisibleType(); //like getType but type of terrain underneath transparent constructions
    unsigned short getGroup();        //group of bare land or the covering construction
    unsigned short getTopGroup();     //group of bare land or the actual construction
    unsigned short getLowerstVisibleGroup(); //like getGroup but group of terrain underneath transparent constructions
    unsigned short getTransportGroup(); //like getGroup but bridges are reported normal transport tiles
    ConstructionGroup* getTileConstructionGroup(); //constructionGroup of the maptile
    ResourceGroup*     getTileResourceGroup();     //resourceGroup of a tile
    ConstructionGroup* getConstructionGroup();     //constructionGroup of maptile or the covering construction
    ConstructionGroup* getTopConstructionGroup();  //constructionGroup of maptile or the actual construction
    ConstructionGroup* getLowerstVisibleConstructionGroup();

    bool is_bare();                    //true if we there is neither a covering construction nor water
    bool is_lake();                    //true on lakes (also under bridges)
    bool is_river();                   //true on rivers (also under bridges)
    bool is_water();                   //true on bridges or lakes (also under bridges)
    bool is_visible();                 //true if tile is not covered by another construction. Only useful for minimap Gameview is rotated to upperleft
    bool is_transport();               //true on tracks, road, rails and bridges
    bool is_residence();               //true if any residence covers the tile
    void writeTemplate();              //create maptile template
    void saveMembers(std::ostream *os);//write maptile AND ground members as XML to stram
};


class MemberRule{
public:
    int memberType; //type of ConstructionMember
    void *ptr; //address of ConstructionMember
};

template <typename MemberType>
class MemberTraits { };

enum Commodity
{
    STUFF_INIT = 0,
    STUFF_FOOD = STUFF_INIT,
    STUFF_JOBS,
    STUFF_COAL,
    STUFF_GOODS,
    STUFF_ORE,
    STUFF_STEEL,
    STUFF_WASTE,
    STUFF_KWH,
    STUFF_MWH,
    STUFF_WATER,
    STUFF_COUNT
};
Commodity& operator++(Commodity& stuff);
Commodity operator++(Commodity& stuff, int);

struct CommodityRule{
    int maxload;
    bool take;
    bool give;
};




class Construction {
public:
    Construction() {
        for(Commodity stuff = STUFF_INIT; stuff < STUFF_COUNT; stuff++)
        {
            commodityCount[stuff] = 0;
            // if(constructionGroup->commodityRuleCount[stuff].maxload)
            //   setMemberSaved(&commodityCount[stuff], commodityNames[stuff]);
            commodityProd[stuff] = 0;
            commodityProdPrev[stuff] = 0;
            commodityMaxProd[stuff] = 0;
            commodityMaxCons[stuff] = 0;
        }
    }
    virtual ~Construction() {}
    virtual void update() = 0;
    virtual void report() = 0;
    virtual void animate() {};
    virtual void initialize() {};


    ConstructionGroup *constructionGroup;
    //ResourceGroup *graphicsGroup;
    ResourceGroup *soundGroup;

    //unsigned short type;
    int x, y;
    int ID;
    int flags;              //flags are defined in lin-city.h

    enum MemberTypes
    {
        TYPE_BOOL,
        TYPE_INT,
        TYPE_USHORT,
        TYPE_DOUBLE,
        TYPE_FLOAT
    };

    // std::map<Commodities, int> commodityCount;  //map that holds all kinds of stuff
    std::array<int, STUFF_COUNT> commodityCount;    // inventory
    std::array<int, STUFF_COUNT> commodityProd;     // production month-to-date
    std::array<int, STUFF_COUNT> commodityProdPrev; // production last month
    std::array<int, STUFF_COUNT> commodityMaxProd;  // max production
    std::array<int, STUFF_COUNT> commodityMaxCons;  // max consumption
    std::map<std::string, MemberRule> memberRuleCount; //what to do with stuff at this construction
    std::vector<Construction*> neighbors;       //adjacent for transport
    std::vector<Construction*> partners;        //remotely for markets
    std::list<ExtraFrame>::iterator frameIt;
    static std::string getStuffName(Commodity stuff_id); //translated name of a commodity
    void init_resources(void);                      //sets sounds and graphics according to constructionGroup
    void list_commodities(int *);                   //prints a sorted list all commodities in report()
    void list_inventory(int *);                     // prints list of commodity inventory in report()
    void list_production(int *);                    // prints list of commodity production in report()
    void reset_prod_counters(void);                 // monthly update to production counters
    int produceStuff(Commodity stuff_id, int amt);  // increases inventory by amt and updates production counter
    int consumeStuff(Commodity stuff_id, int amt);  // decreases inventory by amt and updates production counter
    int levelStuff(Commodity stuff_id, int amt);    // sets inventory level and updates production counter
    void report_commodities(void);                  //adds commodities and capacities to gloabl stat counter
    void initialize_commodities(void);              //sets all commodities to 0 and marks them as saved members
    void bootstrap_commodities(int percentage);     // sets all commodities except STUFF_WASTE to percentage.
    int loadMember(std::string const &xml_tag, std::string const &xml_val);
    int readbinaryMember(std::string const &xml_tag, gzFile fp);
    template <typename MemberType>
    void setMemberSaved(MemberType *ptr, std::string const &xml_tag)
    {
        memberRuleCount[xml_tag].memberType = MemberTraits<MemberType>::TYPE_ID;
        memberRuleCount[xml_tag].ptr = static_cast<void *>(ptr);
    }

    void setCommodityRulesSaved(std::array<CommodityRule,STUFF_COUNT> * stuffRuleCount);
    void writeTemplate();      //create xml template for savegame
    void saveMembers(std::ostream *os);        //writes all needed and optionally set Members as XML to stream
    virtual void place();
    void detach();      //removes all references from world, ::constructionCount
    void deneighborize(); //cancells all neighbors and partners mutually
    void   neighborize(); //adds all neigborconnections once (call twice for double connections)
    int countPowercables(int mask); //removes all but one suspended cable on each edge
    void link_to(Construction* other); //establishes mutual connection to neighbor or partner
    int  tellstuff( Commodity stuff_ID, int level); //tell the filling level of commodity
    void trade(); //exchange commodities with neigbhors
    int equilibrate_stuff(int *rem_lvl, CommodityRule rem_rule , int ratio, Commodity stuff_ID);
    //equilibrates stuff with an external reservoir (e.g. another construction invoking this method)
    void playSound();//plays random chunk from constructionGroup
};

extern const char *commodityNames[];
//global Vars for statistics on commodities
extern std::map<Commodity, int> tstat_capacities;
extern std::map<Commodity, int> tstat_census;

#define MEMBER_TYPE_TRAITS(MemberType, TypeId) \
template <> \
class MemberTraits<MemberType> { \
public: \
    enum { TYPE_ID = TypeId }; \
};

MEMBER_TYPE_TRAITS(int, Construction::TYPE_INT)
MEMBER_TYPE_TRAITS(bool, Construction::TYPE_BOOL)
MEMBER_TYPE_TRAITS(unsigned short, Construction::TYPE_USHORT)
MEMBER_TYPE_TRAITS(double, Construction::TYPE_DOUBLE)
MEMBER_TYPE_TRAITS(float, Construction::TYPE_FLOAT)

template <typename ConstructionClass>
class RegisteredConstruction: public Construction, public Counted<ConstructionClass>
{
public:
    RegisteredConstruction( int x, int y)
    {
        //this->type = 0;//safe default
        this->soundGroup = 0;//to be set by init_resources()
        //this->graphicsGroup = 0;//to be set in ConstgructionGroup::placeItem
        //setMemberSaved(&(this->type),"type");
        this->x = x;
        this->y = y;
        this->ID = Counted<ConstructionClass>::getNextId();
        this->flags = '\0';
        setMemberSaved(&(this->flags),"flags");
#ifdef DEBUG
        neighbors.clear();
        partners.clear();
#endif
    }
    ~RegisteredConstruction(){}
};

class GraphicsInfo
{
    public:
    GraphicsInfo(void){
        texture = (Texture*)'\0';
        image = (SDL_Surface*)'\0';
        x = 0;
        y = 0;
    }

    Texture* texture;
    SDL_Surface* image;
    int x, y;
};

//all instances are added to resMap
class ResourceGroup {
public:

    ResourceGroup(const std::string &tag)
    {
        graphicsInfoVector.clear();
        chunks.clear();
        resourceID = tag;
        images_loaded = false;
        sounds_loaded = false;
        is_vehicle = false;
        //std::cout << "new resourceGroup: " << tag << std::endl;
        if (resMap.count(tag))
        {   std::cout << "rejecting " << tag << " as another ResourceGroup"<< std::endl;}
        else
        {   resMap[tag] = this;}
    }
    ~ResourceGroup()
    {
        std::vector<GraphicsInfo>::iterator it;
        for(it = graphicsInfoVector.begin(); it != graphicsInfoVector.end(); ++it)
        {
            if(it->texture)
            {
                delete it->texture;
                it->texture = 0;
            }
        }
        if ( resMap.count(resourceID))
        {
            resMap.erase(resourceID);
            //std::cout << "sayonara: " << resourceID << std::endl;
        }
        else
        {   std::cout << "error: unreachable resourceGroup: " << resourceID << std::endl;}
    }
    std::string resourceID;
    bool images_loaded;
    bool sounds_loaded;
    bool is_vehicle; //vehicles are always rendered on upper left tile
    std::vector<Mix_Chunk *> chunks;
    std::vector<GraphicsInfo> graphicsInfoVector;
    void growGraphicsInfoVector(void)
    {   graphicsInfoVector.resize(graphicsInfoVector.size() + 1);}
    static std::map<std::string, ResourceGroup*> resMap;
};

class ConstructionGroup {
public:
    ConstructionGroup(
        const char *name,
        bool no_credit,
        unsigned short group,
        unsigned short size, int colour,
        int cost_mul, int bul_cost, int fire_chance,
        int cost, int tech, int range, int mps_pages
    ) {
        this->name = name;
        this->no_credit = no_credit;
        this->group = group;
        this->size = size;
        this->colour = colour;
        this->cost_mul = cost_mul;
        this->bul_cost = bul_cost;
        this->fire_chance = fire_chance;
        this->cost = cost;
        this->tech = tech;
        this->range = range;
        //this->images_loaded = false;
        //this->sounds_loaded = false;
        this->mps_pages = mps_pages;

        for(Commodity stuff = STUFF_INIT; stuff < STUFF_COUNT; stuff++) {
          this->commodityRuleCount[stuff] = (CommodityRule){
            .maxload = 0,
            .take = false,
            .give = false
          };
        }
       }
    virtual ~ConstructionGroup()
    {/*
        std::vector<GraphicsInfo>::iterator it;
        for(it = graphicsInfoVector.begin(); it != graphicsInfoVector.end(); ++it)
        {
            if(it->texture)
            {
                delete it->texture;
                it->texture = 0;
            }
        }*/
    }

    // std::map<Construction::Commodities, CommodityRule> commodityRuleCount;
    std::array<CommodityRule, STUFF_COUNT> commodityRuleCount;
    //std::vector<Mix_Chunk *> chunks;
    //std::vector<GraphicsInfo> graphicsInfoVector;
    int getCosts();
    bool is_allowed_here(int x, int y, bool msg);//check if construction could be placed
    //void growGraphicsInfoVector(void);

    virtual int placeItem(int x, int y);

    // this method must be overriden by the concrete ConstructionGroup classes.
    virtual Construction *createConstruction(int x, int y) = 0;

    std::string getName(void);

    std::string resourceID;           /* name for matching resources from XML*/
    const char *name;           /* inGame name of group */
    bool no_credit;   /* TRUE if need credit to build */
    unsigned short group;       /* This is redundant: it must match
                                   the index into the table */
    unsigned short size;        /* shape in x and y */
    int colour;                 /* summary map colour */
    int cost_mul;               /* group cost multiplier */
    int bul_cost;               /* group bulldoze cost */
    int fire_chance;            /* probability of fire */
    int cost;                   /* group cost */
    int tech;                   /* group tech */
    int range;                  /* range beyond size*/
    //bool images_loaded;
    //bool sounds_loaded;
    int mps_pages;

    static void addConstructionGroup(ConstructionGroup *constructionGroup)
    {
        if ( groupMap.count(constructionGroup->group) )
        {
            std::cout << "rejecting " << constructionGroup->name << " as "
            << constructionGroup->group << " from ConstructionGroup::groupMap"
            << std::endl;
        }
        else
        {   groupMap[constructionGroup->group] = constructionGroup;}


    }

    static void addResourceID(std::string resID, ConstructionGroup *constructionGroup)
    {

        if ( resourceMap.count(resID))
        {
            std::cout << "rejecting " << constructionGroup->name << " as "
            << constructionGroup->resourceID << " from ConstructionGroup::resouceMap"
            << std::endl;
        }
        else
        {
            constructionGroup->resourceID = resID;
            resourceMap[constructionGroup->resourceID] = constructionGroup;
            new ResourceGroup(resID); //adds itself to ResourceGroup::resmap
        }
    }

    static void clearGroupMap()
    {
        // removes all entries from groupMap
        groupMap.clear();
    }

    static void clearResourcepMap()
    {
        // removes all entries from resourceMap
        resourceMap.clear();
    }


    static ConstructionGroup *getConstructionGroup(unsigned short group)
    {
        if (groupMap.count(group))
            return groupMap[group];
        else
            return NULL;
    }

    static void printGroups();

    static int countConstructionGroup(unsigned short group) {
        return groupMap.count(group);
    }

    static std::map<std::string, ConstructionGroup*> resourceMap;
protected:
    // Map associating group ids with the respective construction group objects
    static std::map<unsigned short, ConstructionGroup*> groupMap;

};




struct GROUP {
    const char *name;           // name of group
    unsigned short no_credit;   // TRUE if need credit to build
    unsigned short group;       // This is redundant: it must match
                                // the index into the table
    unsigned short size;
    int colour;                 // summary map colour
    int cost_mul;               // group cost multiplier
    int bul_cost;               // group bulldoze cost
    int fire_chance;            // probability of fire
    int cost;                   // group cost
    int tech;                   // group tech
};


struct TYPE {
    int group;                  // What group does this type belong to?
    char *graphic;              // Bitmap of the graphic
};


#endif /* __lintypes_h__ */

/** @file lincity/lintypes.h */
