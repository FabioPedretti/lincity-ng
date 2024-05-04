#include <array>                    // for array
#include <string>                   // for basic_string

#include "../lintypes.h"            // for tech_level
#include "lincity/all_buildings.h"  // for JOBS_LOAD_COAL, JOBS_LOAD_ORE
#include "lincity/lin-city.h"       // for MAX_TECH_LEVEL
#include "lincity/lintypes.h"       // for CommodityRule, Commodity, Registe...

#define GROUP_INDUSTRY_H_COLOUR (7)     /* colour 7 is an orange, sort of */
#define GROUP_INDUSTRY_H_COST 50000
#define GROUP_INDUSTRY_H_COST_MUL 20
#define GROUP_INDUSTRY_H_BUL_COST 70000
#define GROUP_INDUSTRY_H_TECH 170
#define GROUP_INDUSTRY_H_FIREC 80
#define GROUP_INDUSTRY_H_RANGE 0
#define GROUP_INDUSTRY_H_SIZE 4

#define JOBS_MAKE_STEEL 140
#define ORE_MAKE_STEEL 17
#define POWER_MAKE_STEEL 200
#define COAL_MAKE_STEEL 2
#define POL_PER_STEEL_MADE 0.25
#define MAX_ORE_USED 612
#define MAX_ORE_AT_INDUSTRY_H (20 * MAX_ORE_USED)
#define MAX_COAL_AT_INDUSTRY_H (2 * MAX_ORE_AT_INDUSTRY_H / ORE_MAKE_STEEL)
#define MAX_STEEL_AT_INDUSTRY_H (MAX_ORE_AT_INDUSTRY_H / ORE_MAKE_STEEL)
#define MAX_JOBS_AT_INDUSTRY_H (MAX_ORE_AT_INDUSTRY_H / JOBS_MAKE_STEEL + 20*(JOBS_LOAD_ORE + JOBS_LOAD_COAL + JOBS_LOAD_STEEL))

#define MAX_WASTE_AT_INDUSTRY_H (MAX_STEEL_AT_INDUSTRY_H * POL_PER_STEEL_MADE)
#define MAX_KWH_AT_INDUSTY_H (MAX_STEEL_AT_INDUSTRY_H * POWER_MAKE_STEEL)
#define MAX_MWH_AT_INDUSTY_H (MAX_STEEL_AT_INDUSTRY_H * POWER_MAKE_STEEL)

#define INDUSTRY_H_ANIM_SPEED  290
#define INDUSTRY_H_POLLUTION    10

#define MAX_MADE_AT_INDUSTRY_H 625

class IndustryHeavyConstructionGroup: public ConstructionGroup {
public:
    IndustryHeavyConstructionGroup(
        const char *name,
        unsigned short no_credit,
        unsigned short group,
        unsigned short size, int colour,
        int cost_mul, int bul_cost, int fire_chance,
        int cost, int tech, int range
    ): ConstructionGroup(
        name, no_credit, group, size, colour, cost_mul, bul_cost, fire_chance,
        cost, tech, range, 2/*mps_pages*/
    )
    {
        commodityRuleCount[STUFF_JOBS].maxload = MAX_JOBS_AT_INDUSTRY_H;
        commodityRuleCount[STUFF_JOBS].take = true;
        commodityRuleCount[STUFF_JOBS].give = false;
        commodityRuleCount[STUFF_ORE].maxload = MAX_ORE_AT_INDUSTRY_H;
        commodityRuleCount[STUFF_ORE].take = true;
        commodityRuleCount[STUFF_ORE].give = false;
        commodityRuleCount[STUFF_COAL].maxload = MAX_COAL_AT_INDUSTRY_H;
        commodityRuleCount[STUFF_COAL].take = true;
        commodityRuleCount[STUFF_COAL].give = false;
        commodityRuleCount[STUFF_STEEL].maxload = MAX_STEEL_AT_INDUSTRY_H;
        commodityRuleCount[STUFF_STEEL].take = false;
        commodityRuleCount[STUFF_STEEL].give = true;
        commodityRuleCount[STUFF_WASTE].maxload = MAX_WASTE_AT_INDUSTRY_H;
        commodityRuleCount[STUFF_WASTE].take = false;
        commodityRuleCount[STUFF_WASTE].give = true;
        commodityRuleCount[STUFF_KWH].maxload = MAX_KWH_AT_INDUSTY_H;
        commodityRuleCount[STUFF_KWH].take = true;
        commodityRuleCount[STUFF_KWH].give = false;
        commodityRuleCount[STUFF_MWH].maxload = MAX_MWH_AT_INDUSTY_H;
        commodityRuleCount[STUFF_MWH].take = true;
        commodityRuleCount[STUFF_MWH].give = false;
    };
    // overriding method that creates a HeavyIndustry
    virtual Construction *createConstruction(int x, int y);
};

extern IndustryHeavyConstructionGroup industryHeavyConstructionGroup;
//extern IndustryHeavyConstructionGroup industryHeavy_L_ConstructionGroup;
//extern IndustryHeavyConstructionGroup industryHeavy_M_ConstructionGroup;
//extern IndustryHeavyConstructionGroup industryHeavy_H_ConstructionGroup;

class IndustryHeavy: public RegisteredConstruction<IndustryHeavy> { // IndustryHeavy inherits from its own RegisteredConstruction
public:
    IndustryHeavy(int x, int y, ConstructionGroup *cstgrp): RegisteredConstruction<IndustryHeavy>(x, y)
    {
        constructionGroup = cstgrp;
        init_resources();
        this->tech = tech_level;
        setMemberSaved(&this->tech, "tech");
        this->output_level = 0;
        this->steel_this_month = 0;
        // this->anim = 0;
        initialize_commodities();
         //check for pollution bonus
        this->bonus = 0;
        setMemberSaved(&this->bonus, "bonus"); // compatibility
        this->extra_bonus = 0;
        setMemberSaved(&this->extra_bonus, "extra_bonus"); // compatibility
        // if (tech > MAX_TECH_LEVEL)
        // {
        //     bonus = (tech - MAX_TECH_LEVEL);
        //     if (bonus > MAX_TECH_LEVEL)
        //         bonus = MAX_TECH_LEVEL;
        //     bonus /= MAX_TECH_LEVEL;
        //     // check for filter technology bonus
        //     if (tech > 2 * MAX_TECH_LEVEL)
        //     {
        //         extra_bonus = tech - 2 * MAX_TECH_LEVEL;
        //         if (extra_bonus > MAX_TECH_LEVEL)
        //             extra_bonus = MAX_TECH_LEVEL;
        //         extra_bonus /= MAX_TECH_LEVEL;
        //     }
        // }

        int steel_prod = MAX_ORE_USED / ORE_MAKE_STEEL;
        commodityMaxCons[STUFF_MWH] = 100 * (steel_prod * POWER_MAKE_STEEL / 2);
        commodityMaxCons[STUFF_KWH] = 100 * (steel_prod * POWER_MAKE_STEEL);
        commodityMaxCons[STUFF_COAL] = 100 * (steel_prod * COAL_MAKE_STEEL);
        commodityMaxCons[STUFF_JOBS] = 100 * (MAX_ORE_USED / JOBS_MAKE_STEEL +
          JOBS_LOAD_COAL + JOBS_LOAD_ORE + JOBS_LOAD_STEEL);
        commodityMaxCons[STUFF_ORE] = 100 * MAX_ORE_USED;
        commodityMaxProd[STUFF_STEEL] = 100 * steel_prod;
        // commodityMaxProd[STUFF_WASTE] = 100 * (int)(
        //   ((double)(POL_PER_STEEL_MADE * steel_prod) * bonus)*(1-extra_bonus));
    }

    virtual void initialize() override {
        RegisteredConstruction::initialize();

        if (tech > MAX_TECH_LEVEL)
        {
            bonus = (tech - MAX_TECH_LEVEL);
            if (bonus > MAX_TECH_LEVEL)
                bonus = MAX_TECH_LEVEL;
            bonus /= MAX_TECH_LEVEL;
            // check for filter technology bonus
            if (tech > 2 * MAX_TECH_LEVEL)
            {
                extra_bonus = tech - 2 * MAX_TECH_LEVEL;
                if (extra_bonus > MAX_TECH_LEVEL)
                    extra_bonus = MAX_TECH_LEVEL;
                extra_bonus /= MAX_TECH_LEVEL;
            }
        }

        int steel_prod = MAX_ORE_USED / ORE_MAKE_STEEL;
        commodityMaxProd[STUFF_WASTE] = 100 * (int)(
          ((double)(POL_PER_STEEL_MADE * steel_prod) * bonus)*(1-extra_bonus));
    }

    virtual void update() override;
    virtual void report() override;
    virtual void animate() override;

    int  tech;
    double bonus, extra_bonus;
    int  output_level;
    int  anim;
    int  steel_this_month;
};



/** @file lincity/modules/heavy_industry.h */
