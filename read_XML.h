#ifndef READ_XML_H
#define READ_XML_H

#pragma once
#include "tinyxml/tinyxml.h"
#include "write_XLSX.h"
#include <memory>


class TinyClassObj {
public:
    TinyClassObj(): doc(nullptr),xml_entity(nullptr),SMR(1) {};
    ~TinyClassObj();
    bool loadXmlSourse(const QString way);
    TiXmlElement* getCurrentElement() { return this->xml_entity; }
    TiXmlDocument* getCurrentDoc() { return this->doc; }
    void setNextSiblingElement(const std::string tag);
    void setFirstChildElement(const std::string tag);
    //void setNextDoc(const std::string doc) { this->loadXmlSourse(doc); };
    void goToReadXml();
    void readKoef(TiXmlElement* entity);
    void readPropertyes(TiXmlElement* entity);
    void readWinterIndex(TiXmlElement* entity);
    void readSMR(TiXmlElement* entity);
    void readVibRab(TiXmlElement* entity);
    void fillPositionTag(TiXmlElement* entity, PositionOBJ* tPosition);
    void checkPositionTagForQuantityTag(TiXmlElement* entity, PositionOBJ* tPosition);
    void fillResoursesTag(TiXmlElement* entity, ResourcesOBJ* tResourses);
    void fillPriceBase(TiXmlElement* entity, PositionOBJ* tPosition);
    void fillWorkListTag(TiXmlElement* entity, workListOBJ* tWorkList);
    void calclutePriceBase(PositionOBJ* tPosition, ResourcesOBJ* tResourses);
    void checkNullPriceBase(PositionOBJ* tPosition,double koef_PZ,double koef_OZ,double koef_EM,double koef_ZM,double koef_MT);
    bool checkEqualPriceBase(const PositionOBJ* tPosition);
    void fillLocalKoef(TiXmlElement* entity, PositionOBJ* tPosition);
    void calculatePlanNacl(PositionOBJ* tPosition, const std::vector<vid_Rab>& tVid_Rab, double& OZ, double& ZM, double& koefOZ,double& koefZM);
    const indexes& getIndex() const { return this->koefList; }
    const std::vector<vid_Rab>& getVidRabot() const { return this->tVid_Rab; }
    const std::vector<chapterOBJ>& getChapterList()const { return this->gChaperList; }
    bool checkTheSamePosition(PositionOBJ& tPosition,chapterOBJ& lChapersList);
    void recalcEachKoef(PositionOBJ& tPosition,chapterOBJ& lChapersList);
    const double& getSMR() const { return this->SMR; }
    const std::string& getProperyes()const {return this->propertysOfDocument;};
private:
    TiXmlDocument* doc;
    TiXmlElement* xml_entity;
    indexes koefList;
    std::vector<vid_Rab> tVid_Rab;
    std::vector<chapterOBJ> gChaperList;
    double SMR;
    std::string propertysOfDocument;
};

#endif // READ_XML_H
