#ifndef WRITE_XLSX_H
#define WRITE_XLSX_H

#include "struct_of_XML_source.h"
#include <qaxobject.h>



//position
struct doc_Attribute {
    doc_Attribute():noPos(0), kolvo(0),unit_kolvo(0), matPrice(0), matPriceNds(0), rabPrice(0), rabPriceNds(0), oborPrice(0), oborPriceNds(0), itogo(0), itogoNds(0), smetTrudoemkost(0){};
    std::string noFer;
    std::string noLSR;
    uint16_t noPos;
    std::string namePos;
    std::string edIzm;
    std::string podRazdel;
    double kolvo;
    uint32_t unit_kolvo;
    double matPrice;
    double matPriceNds;
    double rabPrice;
    double rabPriceNds;
    double oborPrice;
    double oborPriceNds;
    double itogo;
    double itogoNds;
    uint16_t smetTrudoemkost;
};
//glava
struct doc_PodRazdel {
    doc_PodRazdel() : itogoMat(0), itogoRab(0), itogoObor(0), itogo(0) {};
    ~doc_PodRazdel() {};
    std::vector<doc_Attribute> atribute;
    std::string nameOfRazdel;
    double itogoMat;
    double itogoRab;
    double itogoObor;
    double itogo;
    double SMR;
};

class doc_Razdel {
public:
    doc_Razdel() : itogoMat(0), itogoRab(0), itogoObor(0), itogo(0) {};
    ~doc_Razdel() {};
    void GomakeXlsx(const std::vector<chapterOBJ>& gChaperList, const std::vector<vid_Rab>& tVid_Rab, const indexes& koefList, const double SMR,const std::string& propertysOfDocument);
    void calculateData(const std::vector<chapterOBJ>& gChaperList,const std::string& propertysOfDocument, const double& smr);
    //void setNameOfGlavi(std::string& tag1, std::string& tag2) {};
private:
    std::vector<doc_PodRazdel> glavi;
    std::string nameOfGlavi;
    double itogoMat;
    double itogoRab;
    double itogoObor;
    double itogo;
};

#endif // WRITE_XLSX_H
