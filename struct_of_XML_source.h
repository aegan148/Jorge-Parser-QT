#ifndef STRUCT_OF_XML_SOURCE_H
#define STRUCT_OF_XML_SOURCE_H

#pragma once
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <QFileDialog>
#include <QDebug>
#include <QTextCodec>

////////////about NAMESPACE//////////
typedef uint8_t index;              //
typedef uint8_t value;
typedef float Quontity;
typedef float PriceBase;
typedef std::string name;
using dataOfTag= std::tuple<name, Quontity, PriceBase>; //
///////////////////////////////////////////////////////////

/////declaration////////////////
struct PositionOBJ;            //
struct ResourcesOBJ;
struct TzmOBJ;
struct TzrOBJ;
struct MchOBJ;
struct MatOBJ;
struct workListOBJ;
struct doc_Attribute;
struct doc_PodRazdel;
class doc_Razdel;
struct vid_Rab;
struct PriceBaseStruct;
struct koef;
struct K_koef_struct;
struct indexes;                 //
////////////////////////////////

struct Podrazdel{
    std::string namePodrazdel;
    uint16_t numPosition;
};

struct ResourcesOBJ {
    std::vector<TzrOBJ> tzr;
    std::vector<TzmOBJ> tzm;
    std::vector<MchOBJ> mch;
    std::vector<MatOBJ> mat;
};
struct chapterOBJ {
    std::vector<PositionOBJ> tPosition;
    std::string chapterName;
    std::vector<Podrazdel> podrazlelList;
};

struct TzmOBJ {
    dataOfTag TagInfo;
};

struct TzrOBJ {
    dataOfTag TagInfo;
};

struct MchOBJ {
    dataOfTag TagInfo;
};

struct MatOBJ {
    dataOfTag TagInfo;
};

struct workListOBJ {
    std::vector<std::string> workList;
};

struct koef {
    koef() : Value_PZ(1), Value_OZ(1), Value_EM(1), Value_ZM(1), Value_MT(1) {};
    ~koef() {};
    double Value_PZ;
    double Value_OZ;
    double Value_EM;
    double Value_ZM;
    double Value_MT;
};
struct K_koef_struct {
    K_koef_struct() : VrsLinks(0), ChaptersLinks(0), level(0) {};
    koef K_Koefs;
    uint32_t VrsLinks;
    uint32_t ChaptersLinks;
    uint32_t level;
};

struct indexes {
    indexes() : IswinterKoef(false), IsK_Koef(false) {};
    ~indexes() {};
    bool IswinterKoef;
    bool IsK_Koef;
    std::vector<K_koef_struct> K_Koefs;
    std::vector<koef> winterKoefs;
};

struct PriceBaseStruct {
    PriceBaseStruct() :PZ(0), OZ(0), MT(0), EM(0), ZM(0) {};
    ~PriceBaseStruct() {};
    double PZ;
    double OZ;
    double EM;
    double ZM;
    double MT;   
};

struct vid_Rab {
    vid_Rab(): Nacl(0), Plan(0), id(0), calculated(false) {};
    ~vid_Rab() {};
    double Nacl;
    double Plan;
    uint16_t id;
    bool calculated;
};

struct PositionOBJ {
    PositionOBJ() :Dont_add(false), badCalculate(false),koef_each(false), KUnit(1), number(0), indexCode(0),  Vr2001(0),num_Podrazdel(0),Quantity(0), matPriceNdsCalculated(0) {};
    ~PositionOBJ() {};
    bool Dont_add;
    bool badCalculate;
    bool koef_each;
    uint32_t KUnit;
    uint32_t number;
    uint16_t indexCode;
    uint16_t  Vr2001;
    uint16_t num_Podrazdel;
    double Quantity;
    double matPriceNdsCalculated;
    std::string Podrazdel;
    std::string Caption;
    std::string Code;
    std::string FER_index;
    std::string Units;
    std::string each_FER_index;
    PriceBaseStruct tPositionPriceBaseReaded;           //это и ниже потом сравниваются
    PriceBaseStruct tPositionPriceBaseCalculated;           //вот это сравнивается
    vid_Rab tPositionCalculatedVidRab;
    std::vector<ResourcesOBJ> tPositionResources;
    workListOBJ tPositionWorkList;
    koef localKoef;
};

#endif // STRUCT_OF_XML_SOURCE_H
