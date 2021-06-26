#include "read_XML.h"
#include <QTextCodec>
#include <QMessageBox>
#include <math.h>


std::string convertQstringToString(const char* ss){
    QString str;
    std::string result;
    QTextCodec *codec = QTextCodec::codecForName("CP1251"); //Вот оно, тут
    assert(codec); // И тут
    str = codec->toUnicode(ss); // И здесь
    result = str.toUtf8().constData();
    return result;
}
template <typename to, typename from>
to StringToNumber(const from& rhs) {
    to result;
    std::stringstream os;
    try {
        os << rhs;
        os >> result;
        if (os.fail()) {
            std::runtime_error e("eror cast");
            throw e;
        }
    }
    catch (std::exception& ex) { return 0; }
    return result;
};

double operator>>(const TzrOBJ& src, double result) {
    double quantity = 0;
    double value = 0;

    quantity = std::get<1>(src.TagInfo);
    value = std::get<2>(src.TagInfo);
    return result = quantity * value;
}
double operator>>(const MchOBJ& src, double result) {
    double quantity = 0;
    double value = 0;

    quantity = std::get<1>(src.TagInfo);
    value = std::get<2>(src.TagInfo);
    return result = quantity * value;
}
double operator>>(const MatOBJ& src, double result) {
    double quantity = 0;
    double value = 0;

    quantity = std::get<1>(src.TagInfo);
    value = std::get<2>(src.TagInfo);
    return result = quantity * value;
}
double operator>>(const TzmOBJ& src, double result) {
    double quantity = 0;
    double value = 0;

    quantity = std::get<1>(src.TagInfo);
    value = std::get<2>(src.TagInfo);
    return result = quantity * value;
}

template <typename summ, typename from>
summ getSumofResources(const std::vector<from>& vect, double kolvo, double koef_MT, double koef_PZ) {
    summ tsum = 0; //double
    summ temp=0;
    for (const auto f : vect) {
        temp = f >> temp;
          temp = int(temp*100 +0.5)/100.0;
          temp *= koef_PZ;
          temp = int(temp*100 +0.5)/100.0;
          temp *= koef_MT;
          temp = int(temp*100 +0.5)/100.0;
          temp *= kolvo;
          temp = int(temp*100 +0.5)/100.0;
          tsum+=temp;
    }
    summ b = int(tsum*100 +0.5)/100.0;
    return b;
}

template <typename summ, typename from>
summ getSumofResources(const std::vector<from>& vect) {
    summ tsum = 0; //double
    for (const auto f : vect) {
        tsum += f >> tsum;
    }
    summ b = int(tsum*100 +0.5)/100.0;
    return b;
}


TinyClassObj::~TinyClassObj()
{
  //  if (doc) { delete doc; }
   // if (xml_entity) { delete xml_entity; }
}

bool TinyClassObj::loadXmlSourse(const QString way) {
    QByteArray ba = way.toLocal8Bit();
    const char *c_str2 = ba.data();
    this->doc = new TiXmlDocument(c_str2);
    bool res;
    (this->doc->LoadFile() == 0) ? res=false : res=true;
    if (res) { this->xml_entity = this->doc->FirstChildElement("Document"); }
    return res;
}

void TinyClassObj::setNextSiblingElement(const std::string tag)
{
    this->xml_entity = this->xml_entity->NextSiblingElement(tag.c_str());
}

void TinyClassObj::setFirstChildElement(const std::string tag)
{
    this->xml_entity = this->xml_entity->FirstChildElement(tag.c_str());
}

void TinyClassObj::goToReadXml()
{
    TiXmlElement* LocalEntity=NULL;          //есть необходимость захода в атрибуты именно по локальному объекту тини
    std::string tempStr;

    if (this->xml_entity->FirstChildElement("Properties")) {
        LocalEntity = this->xml_entity->FirstChildElement("Properties");
        this->readPropertyes(LocalEntity);
    }
    if (this->xml_entity->FirstChildElement("Koefficients")) {
        LocalEntity = this->xml_entity->FirstChildElement("Koefficients");
        this->readKoef(LocalEntity);
    }
    if (this->xml_entity->FirstChildElement("WinterCatalog")) {
        LocalEntity = this->xml_entity->FirstChildElement("WinterCatalog");
        this->readWinterIndex(LocalEntity);
    }
    if (this->xml_entity->FirstChildElement("Indexes")) {
        LocalEntity = this->xml_entity->FirstChildElement("Indexes");
        this->readSMR(LocalEntity);
    }
    ///////////////////////////////////////////////////////////////////////////////
    if (this->xml_entity->FirstChildElement("VidRab_Catalog")) {
        LocalEntity = this->xml_entity->FirstChildElement("VidRab_Catalog");
        this->readVibRab(LocalEntity);
    };
    if (this->xml_entity->FirstChildElement("Chapters")) {
        LocalEntity = this->xml_entity->FirstChildElement("Chapters");
    };
    TiXmlElement* chapterSSEntity= LocalEntity;
    chapterSSEntity = chapterSSEntity->FirstChildElement("Chapter");
    if (chapterSSEntity)
    {
        doc_PodRazdel chapterToDoc;
        TiXmlElement* chapterEntity= chapterSSEntity;
        while (chapterSSEntity){
            chapterOBJ lChapersList; // in the end makes push back
            if (chapterSSEntity->Attribute("Caption")) {
                lChapersList.chapterName= convertQstringToString( chapterSSEntity->Attribute("Caption"));
            }
            chapterEntity = chapterSSEntity->FirstChildElement("Position");
            TiXmlElement* PositionEntity = chapterEntity;
            while (chapterEntity) {
                PositionOBJ tPosition;
                if (PositionEntity)
                {
                    fillPositionTag(PositionEntity, &tPosition);
                    checkPositionTagForQuantityTag(PositionEntity, &tPosition);
                }
                if (PositionEntity->FirstChildElement("PriceBase")) { PositionEntity = PositionEntity->FirstChildElement("PriceBase"); }
                if (PositionEntity)
                {
                    fillPriceBase(PositionEntity, &tPosition);
                }
                ResourcesOBJ tResourses;
                if (PositionEntity->NextSiblingElement("Resources")) { PositionEntity = PositionEntity->NextSiblingElement("Resources"); }
                if (PositionEntity)
                {
                    fillResoursesTag(PositionEntity, &tResourses);
                    /*calclutePriceBase(&tPosition, &tResourses);
                    checkNullPriceBase(&tPosition);
                    tPosition.badCalculate = checkEqualPriceBase(&tPosition);
                    tPosition.tPositionResources.push_back(tResourses);*/
                }
                if (PositionEntity->NextSiblingElement("Koefficients")) { PositionEntity = PositionEntity->NextSiblingElement("Koefficients"); }
                if (PositionEntity)
                {
                    fillLocalKoef(PositionEntity, &tPosition);
                    calclutePriceBase(&tPosition, &tResourses); //
                    checkNullPriceBase(&tPosition, tPosition.localKoef.Value_PZ,tPosition.localKoef.Value_OZ,tPosition.localKoef.Value_EM,tPosition.localKoef.Value_ZM,tPosition.localKoef.Value_MT);
                    tPosition.badCalculate = false; // TO DO  checkEqualPriceBase(&tPosition);
                    tPosition.tPositionResources.push_back(tResourses);//
                    if(tPosition.badCalculate){
                        QString warn= tPosition.Caption.c_str();
                        warn +=" \n Пересчитанное значение тега PriceBase,""\n не совпадает с прочитанным. Результат может быть некорректным";
                        QMessageBox::warning(0,"Предупреждение",warn,"Ok",QString(), 0, 1);}
                    calculatePlanNacl(&tPosition, this->tVid_Rab, tPosition.tPositionPriceBaseCalculated.OZ, tPosition.tPositionPriceBaseCalculated.ZM, tPosition.localKoef.Value_OZ,tPosition.localKoef.Value_ZM);
                }
                workListOBJ tWorkList;
                if (PositionEntity->NextSiblingElement("WorksList")) { PositionEntity = PositionEntity->NextSiblingElement("WorksList"); }
                if (PositionEntity)
                {
                    fillWorkListTag(PositionEntity, &tWorkList);
                }
                tPosition.tPositionWorkList = tWorkList;
                lChapersList.tPosition.push_back(tPosition);
                chapterEntity = chapterEntity->NextSiblingElement("Position");
                PositionEntity = chapterEntity;
            }
            recalculate_koef_EACH(lChapersList.tPosition);
            this->gChaperList.push_back(lChapersList);
            chapterSSEntity = chapterSSEntity->NextSiblingElement("Chapter");
        }
    }
}

void TinyClassObj::recalculate_koef_EACH(std::vector<PositionOBJ>& tPosition){

    std::vector<uint32_t> positions;

    for(auto& x: tPosition){
        if (x.koef_each){
            positions.push_back(x.number);
            x.koef_each =false;
        }
    }
    for(auto& x: tPosition){
        for(auto & y: positions){
            uint32_t value_Minus= y-1;
            if (x.number == value_Minus){
                x.koef_each =true;
            }
        }
    }

}

void readNumberOfVrsLinks(indexes& MainkoefList, K_koef_struct& tKoef, std::string& str) {
    auto op = [](const char c) { return c != ' ' && !isdigit(c); };
    auto last = std::remove_if(str.begin(), str.end(), op);
    std::istringstream is(std::string(str.begin(), last));
    uint32_t n;
    while (is >> n) {
        tKoef.VrsLinks = n;
        MainkoefList.K_Koefs.push_back(tKoef);
    }
}
void readNumberOfChapters(K_koef_struct& tKoef, std::string& str) {
    auto op = [](const char c) { return c != ' ' && !isdigit(c); };
    auto last = std::remove_if(str.begin(), str.end(), op);
    std::istringstream is(std::string(str.begin(), last));
    uint32_t n;
    while (is >> n) {
        tKoef.ChaptersLinks = n;
    }
}

void TinyClassObj::readPropertyes(TiXmlElement* entity){
    TiXmlElement* level = entity;               ///////////////////////////////////////////////////////////////////////////////correct read attribute
    if (level->Attribute("LocNum")) {
        this->propertysOfDocument= convertQstringToString(level->Attribute("LocNum"));
    }

}

void TinyClassObj::readKoef(TiXmlElement* entity)
{
    TiXmlElement* level = entity;               ///////////////////////////////////////////////////////////////////////////////correct read attribute
    std::string ss;
    level = level->FirstChildElement("K");
    while (level) {
        K_koef_struct tKoef;
        TiXmlElement* lowlevel = level;
        if (lowlevel->Attribute("ChaptersLinks")) {
            ss = lowlevel->Attribute("ChaptersLinks");
            readNumberOfChapters(tKoef, ss);                //нет чтения нескольких глав
        }
        if (lowlevel->Attribute("Level")) {
            ss = lowlevel->Attribute("Level");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tKoef.level = StringToNumber<uint32_t, std::string>(ss);
        }
        if (lowlevel->Attribute("Value_OZ")) {
            ss=lowlevel->Attribute("Value_OZ");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tKoef.K_Koefs.Value_OZ=StringToNumber<double, std::string>(ss);
        }
        if (lowlevel->Attribute("Value_EM")) {
            ss = lowlevel->Attribute("Value_EM");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tKoef.K_Koefs.Value_EM= StringToNumber<double, std::string>(ss);
        }
        if (lowlevel->Attribute("Value_PZ")) {
            ss = lowlevel->Attribute("Value_PZ");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tKoef.K_Koefs.Value_PZ=StringToNumber<double, std::string>(ss);
        }
        if (lowlevel->Attribute("Value_ZM")) {
            ss = lowlevel->Attribute("Value_ZM");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tKoef.K_Koefs.Value_ZM = StringToNumber<double, std::string>(ss);
        }
        if (lowlevel->Attribute("VrsLinks")) {
            ss = lowlevel->Attribute("VrsLinks");
            readNumberOfVrsLinks(this->koefList, tKoef, ss);
        }
        else {
            this->koefList.K_Koefs.push_back(tKoef);            //на все линки распространяется кеф
        }
        level = level->NextSiblingElement("K");
        this->koefList.IsK_Koef = true;
    }
}
void TinyClassObj::fillLocalKoef(TiXmlElement* entity, PositionOBJ* tPosition)
{
    TiXmlElement* level = entity;
    std::string ss;
    if (level) {
        level = level->FirstChildElement("K");
        while (level != NULL) {
            if (level->Attribute("Value_OZ")) {
                ss = level->Attribute("Value_OZ");
                std::replace(ss.begin(), ss.end(), ',', '.');
                tPosition->localKoef.Value_OZ = StringToNumber<double, std::string>(ss);
            }
            if (level->Attribute("Value_EM")) {
                ss = level->Attribute("Value_EM");
                std::replace(ss.begin(), ss.end(), ',', '.');
                tPosition->localKoef.Value_EM = StringToNumber<double, std::string>(ss);
            }
            if (level->Attribute("Value_MT")) {
                ss = level->Attribute("Value_MT");
                std::replace(ss.begin(), ss.end(), ',', '.');
                tPosition->localKoef.Value_MT = StringToNumber<double, std::string>(ss);
            }
            if (level->Attribute("Value_PZ")) {
                ss = level->Attribute("Value_PZ");
                std::replace(ss.begin(), ss.end(), ',', '.');
                tPosition->localKoef.Value_PZ = StringToNumber<double, std::string>(ss);
            }
            if (level->Attribute("Value_ZM")) {
                ss = level->Attribute("Value_ZM");
                std::replace(ss.begin(), ss.end(), ',', '.');
                tPosition->localKoef.Value_ZM = StringToNumber<double, std::string>(ss);
            }
            level = level->NextSiblingElement("K");
        }
    }
}

void TinyClassObj::readWinterIndex(TiXmlElement* entity)
{
    std::string ss;
    koef tWinterKoef;
    if (entity->Attribute("WinterMode")) {
        ss = entity->Attribute("WinterMode");
    }
        if (ss != "None") {
            this->koefList.IswinterKoef = true;
            /// to be continue
        }
        else { this->koefList.IswinterKoef = false; }
}

void TinyClassObj::readSMR(TiXmlElement* entity)
{
    TiXmlElement* level = entity;               ///////////////////////////////////////////////////////////////////////////////correct read attributeIndexesMode
    std::string ss;
    level = entity->FirstChildElement("IndexesPos");
    level = level->FirstChildElement("Index");
    if (level->Attribute("SMR")) {
        ss = level->Attribute("SMR");
        std::replace(ss.begin(), ss.end(), ',', '.');
        this->SMR = StringToNumber<double, std::string>(ss);
    }
}

void TinyClassObj::readVibRab(TiXmlElement* entity)
{
    std::string ss;
    TiXmlElement* level = entity;
    level = level->FirstChildElement("Vids_Rab");
    level = level->FirstChildElement("VidRab_Group");
    while (level) {
        TiXmlElement* lowlevel = level;
        vid_Rab tmpVidRab;
        lowlevel = lowlevel->FirstChildElement("Vid_Rab");
        while (lowlevel) {
            if (lowlevel->Attribute("ID")) {
                ss = lowlevel->Attribute("ID");
                tmpVidRab.id = StringToNumber<uint16_t, std::string>(ss);
            }
            if (lowlevel->Attribute("Nacl")) {
                ss = lowlevel->Attribute("Nacl");
                tmpVidRab.Nacl = StringToNumber<double, std::string>(ss);
            }
            else { tmpVidRab.Nacl = 1; }

            if (lowlevel->Attribute("Plan")) {
                ss = lowlevel->Attribute("Plan");
                tmpVidRab.Plan = StringToNumber<double, std::string>(ss);
            }
            else { tmpVidRab.Plan = 1; }
            this->tVid_Rab.push_back(tmpVidRab);
            lowlevel = lowlevel->NextSiblingElement("Vid_Rab");
        }
        level = level->NextSiblingElement("VidRab_Group");
    }
}
#include <algorithm>
void TinyClassObj::fillPositionTag(TiXmlElement* entity, PositionOBJ* tPosition)
{
    TiXmlElement* level = entity;

    std::string ss;
    if(level->Attribute("Caption")){
        tPosition->Caption=convertQstringToString(level->Attribute("Caption"));       

        std::string word=" на каждый";
        int pos = tPosition->Caption.find(word); // поиск

        if (pos != -1)
        {
            tPosition->koef_each=true;
        }

    }
    if(level->Attribute("Code")){
        tPosition->Code=convertQstringToString(level->Attribute("Code"));
    }
    if(level->Attribute("Options")){
        std::string tmp;
        tmp=convertQstringToString(level->Attribute("Options"));
        if(tmp == "Inactive") tPosition->Dont_add  =true;

    }
    if(level->Attribute("Units")){

        ss=convertQstringToString(level->Attribute("Units"));
        QString sss(ss.c_str());
        int point=0;
        for(int i=0; i< sss.size();i++){
            if((sss[i] >= L'А' && sss[i] <= L'Я') || (sss[i] >= L'а' && sss[i] <= L'я')){
                point=i;
                break;
            }
        }
        std::copy(ss.begin() + point, ss.end(), std::back_inserter(tPosition->Units));

    }
    if ( level->Attribute("Number")) {
        ss = level->Attribute("Number");
        tPosition->number = StringToNumber<uint32_t, std::string>(ss);
    }
    if ( level->Attribute("IndexCode")) {
        ss = level->Attribute("IndexCode");
        tPosition->indexCode = StringToNumber<uint16_t, std::string>(ss);
    }
    else { tPosition->indexCode = 0; }
    if (level->Attribute("Quantity")) {
        ss = level->Attribute("Quantity");
        std::replace(ss.begin(), ss.end(), ',', '.');
        tPosition->Quantity = StringToNumber<double, std::string>(ss);
    }
    else { tPosition->Quantity = 1; }
    if (level->Attribute("Vr2001")) {
        ss = level->Attribute("Vr2001");
        tPosition->Vr2001 = StringToNumber <uint16_t, std::string>(ss);
    }
    else { tPosition->Vr2001 = 0; }
}

void TinyClassObj::checkPositionTagForQuantityTag(TiXmlElement* entity, PositionOBJ* tPosition)
{
    TiXmlElement* level = entity;
    std::string ss;
    try {
        level = level->FirstChildElement("Quantity");
        if (level) {
            if (level->Attribute("Result")) {
                ss = level->Attribute("Result");
                std::replace(ss.begin(), ss.end(), ',', '.');
                tPosition->Quantity = StringToNumber<double, std::string>(ss);
            }
            if (level->Attribute("KUnit")) {
                ss = level->Attribute("KUnit");
                tPosition->KUnit = StringToNumber<uint32_t, std::string>(ss);
            }
            else { std::runtime_error ex("There is not resalt into the Quantity"); throw ex; }
        }
    }
    catch (std::exception& ex) {
    ///
    }
}

void TinyClassObj::fillResoursesTag(TiXmlElement* entity, ResourcesOBJ* tResourses)
{
    TzrOBJ lTzr;
    MchOBJ lMch;
    MatOBJ lMat;
    TiXmlElement* level=entity;
    TiXmlElement* Aentity = level;
    std::string ss, tCaption;
    dataOfTag tempTuple;
    float tQuont, tPB;
    level = entity->FirstChildElement("Mat");
    while (level) {
        tCaption = level->Attribute("Caption");
        if (level->Attribute("Quantity")) {
            ss = level->Attribute("Quantity");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tQuont = StringToNumber<float, std::string>(ss);
        }
        else {
            tQuont = 1;
        }
        if (level->FirstChildElement("PriceBase")) {
            Aentity = level->FirstChildElement("PriceBase");
            ss = Aentity->Attribute("Value");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tPB = StringToNumber<float, std::string>(ss);
        }
        else {
            tPB = 0;
        }
        if (level->Attribute("Options")) {
            tPB = 0;
            tQuont = 1;
        }
        tempTuple = std::make_tuple(tCaption, tQuont, tPB);
        lMat.TagInfo = tempTuple;
        tResourses->mat.push_back(lMat);
        level = level->NextSiblingElement("Mat");
    }
    level = entity->FirstChildElement("Mch");
    while (level) {
        tCaption = level->Attribute("Caption");
        if (level->Attribute("Quantity")) {
            ss = level->Attribute("Quantity");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tQuont = StringToNumber<float, std::string>(ss);
        }
        else {
            tQuont = 1;
        }
        if (level->FirstChildElement("PriceBase")) {
            Aentity = level->FirstChildElement("PriceBase");
            ss = Aentity->Attribute("Value");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tPB = StringToNumber<float, std::string>(ss);
        }
        else {
            tPB = 0;
        }
        if (level->Attribute("Options")) {
            tPB = 0;
            tQuont = 1;
        }
        tempTuple = std::make_tuple(tCaption, tQuont, tPB);
        lMch.TagInfo = tempTuple;
        tResourses->mch.push_back(lMch);
        level = level->NextSiblingElement("Mch");
    }
    level = entity->FirstChildElement("Tzr");
    while (level) {
        tCaption = level->Attribute("Caption");
        if (level->Attribute("Quantity")) {
            ss = level->Attribute("Quantity");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tQuont = StringToNumber<float, std::string>(ss);
        }
        else {
            tQuont = 1;
        }
        if (level->FirstChildElement("PriceBase")) {
            Aentity = level->FirstChildElement("PriceBase");
            ss = Aentity->Attribute("Value");
            std::replace(ss.begin(), ss.end(), ',', '.');
            tPB = StringToNumber<float, std::string>(ss);
        }
        else {
            tPB = 0;
        }
        if (level->Attribute("Options")) {
            tPB = 0;
            tQuont = 1;
        }
        tempTuple = std::make_tuple(tCaption, tQuont, tPB);
        lTzr.TagInfo = tempTuple;
        tResourses->tzr.push_back(lTzr);
        level = level->NextSiblingElement("Tzr");
    }
}


void TinyClassObj::fillPriceBase(TiXmlElement* entity, PositionOBJ* tPosition)
{
    TiXmlElement* level = entity;
    std::string ss;
    if (level->Attribute("PZ")) {
        ss = level->Attribute("PZ");
        std::replace(ss.begin(), ss.end(), ',', '.');
        tPosition->tPositionPriceBaseReaded.PZ = StringToNumber<double, std::string>(ss);
    }
    else { tPosition->tPositionPriceBaseReaded.PZ = 0; }
    if (level->Attribute("OZ")) {
        ss = level->Attribute("OZ");
        std::replace(ss.begin(), ss.end(), ',', '.');
        tPosition->tPositionPriceBaseReaded.OZ = StringToNumber<double, std::string>(ss);
    }
    else { tPosition->tPositionPriceBaseReaded.OZ = 0; }
    if(level->Attribute("MT")) {
        ss = level->Attribute("MT");
        std::replace(ss.begin(), ss.end(), ',', '.');
        tPosition->tPositionPriceBaseReaded.MT = StringToNumber<double, std::string>(ss);
    }
    else { tPosition->tPositionPriceBaseReaded.MT = 0; }
    if(level->Attribute("EM")) {
        ss = level->Attribute("EM");
        std::replace(ss.begin(), ss.end(), ',', '.');
        tPosition->tPositionPriceBaseReaded.EM = StringToNumber<double, std::string>(ss);
    }
    else { tPosition->tPositionPriceBaseReaded.EM = 0; }
    if(level->Attribute("ZM")) {
        ss = level->Attribute("ZM");
        std::replace(ss.begin(), ss.end(), ',', '.');
        tPosition->tPositionPriceBaseReaded.ZM = StringToNumber<double, std::string>(ss);
    }
    else { tPosition->tPositionPriceBaseReaded.ZM = 0; }
}

void TinyClassObj::fillWorkListTag(TiXmlElement* entity, workListOBJ* tWorkList)
{
    TiXmlElement* level = entity;
    std::string ss;
    level = level->FirstChildElement("Work");
    while (level) {
        if (level->Attribute("Caption")) {
            ss = convertQstringToString( level->Attribute("Caption"));
            tWorkList->workList.push_back(ss);
        }
        level=level->NextSiblingElement("Work");
    }
}

void TinyClassObj::calclutePriceBase(PositionOBJ* tPosition, ResourcesOBJ* tResourses)
{   
    tPosition->tPositionPriceBaseCalculated.OZ = getSumofResources<double, TzrOBJ>(tResourses->tzr);
    tPosition->tPositionPriceBaseCalculated.OZ *= tPosition->localKoef.Value_PZ;
    tPosition->tPositionPriceBaseCalculated.OZ=floor( tPosition->tPositionPriceBaseCalculated.OZ*100 +0.5)/100;
    tPosition->tPositionPriceBaseCalculated.OZ *= tPosition->localKoef.Value_OZ;
    tPosition->tPositionPriceBaseCalculated.OZ=floor( tPosition->tPositionPriceBaseCalculated.OZ*100 +0.5)/100;


    tPosition->tPositionPriceBaseCalculated.MT = getSumofResources<double, MatOBJ>(tResourses->mat);
    tPosition->tPositionPriceBaseCalculated.MT *= tPosition->localKoef.Value_PZ;
    tPosition->tPositionPriceBaseCalculated.MT=floor( tPosition->tPositionPriceBaseCalculated.MT*100 +0.5)/100;
    tPosition->tPositionPriceBaseCalculated.MT *= tPosition->localKoef.Value_MT;
    tPosition->tPositionPriceBaseCalculated.MT=floor( tPosition->tPositionPriceBaseCalculated.MT*100 +0.5)/100;


    tPosition->tPositionPriceBaseCalculated.EM = getSumofResources<double, MchOBJ>(tResourses->mch);
    tPosition->tPositionPriceBaseCalculated.EM *= tPosition->localKoef.Value_PZ;
    tPosition->tPositionPriceBaseCalculated.EM =floor( tPosition->tPositionPriceBaseCalculated.EM*100 +0.5)/100;
    tPosition->tPositionPriceBaseCalculated.EM *= tPosition->localKoef.Value_EM;
    tPosition->tPositionPriceBaseCalculated.EM =floor( tPosition->tPositionPriceBaseCalculated.EM*100 +0.5)/100;


    tPosition->tPositionPriceBaseCalculated.ZM = getSumofResources<double, TzmOBJ>(tResourses->tzm);
    tPosition->tPositionPriceBaseCalculated.ZM *= tPosition->localKoef.Value_PZ;
    tPosition->tPositionPriceBaseCalculated.ZM =floor( tPosition->tPositionPriceBaseCalculated.ZM*100 +0.5)/100;
    tPosition->tPositionPriceBaseCalculated.ZM *= tPosition->localKoef.Value_ZM;
    tPosition->tPositionPriceBaseCalculated.ZM =floor( tPosition->tPositionPriceBaseCalculated.ZM*100 +0.5)/100;


    tPosition->tPositionPriceBaseCalculated.PZ = tPosition->tPositionPriceBaseCalculated.EM + tPosition->tPositionPriceBaseCalculated.MT + tPosition->tPositionPriceBaseCalculated.OZ;

    if(!tPosition->tPositionPriceBaseCalculated.MT){
        tPosition->matPriceNdsCalculated = tPosition->tPositionPriceBaseReaded.MT * tPosition->Quantity;
        tPosition->matPriceNdsCalculated =floor( tPosition->matPriceNdsCalculated*100 +0.5)/100;
    }
    else{
        if(tPosition->number == 13){
            std::cout << "gfd";

        }
        tPosition->matPriceNdsCalculated = getSumofResources<double, MatOBJ>(tResourses->mat, tPosition->Quantity,tPosition->localKoef.Value_MT,tPosition->localKoef.Value_PZ);
    }
}

void TinyClassObj::checkNullPriceBase(PositionOBJ* tPosition, double koef_PZ,double koef_OZ,double koef_EM,double koef_ZM,double koef_MT)
{
    if (tPosition->tPositionPriceBaseCalculated.EM == 0) {
        tPosition->tPositionPriceBaseCalculated.EM = tPosition->tPositionPriceBaseReaded.EM;
        tPosition->tPositionPriceBaseCalculated.EM *= koef_PZ;
        tPosition->tPositionPriceBaseCalculated.EM =floor( tPosition->tPositionPriceBaseCalculated.EM*100 +0.5)/100;
        tPosition->tPositionPriceBaseCalculated.EM *= koef_EM;
        tPosition->tPositionPriceBaseCalculated.EM =floor( tPosition->tPositionPriceBaseCalculated.EM*100 +0.5)/100;
    }
    if (tPosition->tPositionPriceBaseCalculated.MT == 0) {
        tPosition->tPositionPriceBaseCalculated.MT = tPosition->tPositionPriceBaseReaded.MT;
        tPosition->tPositionPriceBaseCalculated.MT *= koef_PZ;
        tPosition->tPositionPriceBaseCalculated.MT =floor( tPosition->tPositionPriceBaseCalculated.MT*100 +0.5)/100;
        tPosition->tPositionPriceBaseCalculated.MT *= koef_MT;
        tPosition->tPositionPriceBaseCalculated.MT =floor( tPosition->tPositionPriceBaseCalculated.MT*100 +0.5)/100;
    }
    if (tPosition->tPositionPriceBaseCalculated.OZ == 0) {
        tPosition->tPositionPriceBaseCalculated.OZ = tPosition->tPositionPriceBaseReaded.OZ;
        tPosition->tPositionPriceBaseCalculated.OZ *= koef_PZ;
        tPosition->tPositionPriceBaseCalculated.OZ =floor( tPosition->tPositionPriceBaseCalculated.OZ*100 +0.5)/100;
        tPosition->tPositionPriceBaseCalculated.OZ *= koef_OZ;
        tPosition->tPositionPriceBaseCalculated.OZ =floor( tPosition->tPositionPriceBaseCalculated.OZ*100 +0.5)/100;
    }
    if (tPosition->tPositionPriceBaseCalculated.ZM == 0) {
        tPosition->tPositionPriceBaseCalculated.ZM = tPosition->tPositionPriceBaseReaded.ZM;
        tPosition->tPositionPriceBaseCalculated.ZM *= koef_PZ;
        tPosition->tPositionPriceBaseCalculated.ZM =floor( tPosition->tPositionPriceBaseCalculated.ZM*100 +0.5)/100;
        tPosition->tPositionPriceBaseCalculated.ZM *= koef_ZM;
        tPosition->tPositionPriceBaseCalculated.ZM =floor( tPosition->tPositionPriceBaseCalculated.ZM*100 +0.5)/100;
    }
    tPosition->tPositionPriceBaseCalculated.PZ = tPosition->tPositionPriceBaseCalculated.EM + tPosition->tPositionPriceBaseCalculated.MT + tPosition->tPositionPriceBaseCalculated.OZ;
}

bool TinyClassObj::checkEqualPriceBase(const PositionOBJ* tPosition)
{
    std::stringstream ss;
    try {
        if ((tPosition->tPositionPriceBaseCalculated.EM - tPosition->tPositionPriceBaseReaded.EM) > 0.1) {
            ss << tPosition->Caption; ss << std::endl;
            ss << "EM calculated isn't equal EM readed...";
            ss << tPosition->tPositionPriceBaseCalculated.EM;
            ss << " != ";
            ss << tPosition->tPositionPriceBaseReaded.EM;
            std::runtime_error ex(ss.str());
            throw ex;
        }
        if ((tPosition->tPositionPriceBaseCalculated.MT - tPosition->tPositionPriceBaseReaded.MT) > 0.1) {
            ss << tPosition->Caption; ss << std::endl;
            ss << "MT calculated isn't equal MT readed...";
            ss << tPosition->tPositionPriceBaseCalculated.MT;
            ss << " != ";
            ss << tPosition->tPositionPriceBaseReaded.MT;
            std::runtime_error ex(ss.str());
            throw ex;
        }
        if ((tPosition->tPositionPriceBaseCalculated.OZ - tPosition->tPositionPriceBaseReaded.OZ) > 0.1) {
            ss << tPosition->Caption; ss << std::endl;
            ss << "OZ calculated isn't equal OZ readed...";
            ss << tPosition->tPositionPriceBaseCalculated.OZ;
            ss << " != ";
            ss << tPosition->tPositionPriceBaseReaded.OZ;
            std::runtime_error ex(ss.str());
            throw ex;
        }
        if ((tPosition->tPositionPriceBaseCalculated.ZM - tPosition->tPositionPriceBaseReaded.ZM) > 0.1) {
            ss << tPosition->Caption; ss << std::endl;
            ss << "ZM calculated isn't equal ZM readed...";
            ss << tPosition->tPositionPriceBaseCalculated.ZM;
            ss << " != ";
            ss << tPosition->tPositionPriceBaseReaded.ZM;
            std::runtime_error ex(ss.str());
        }
        if ((tPosition->tPositionPriceBaseCalculated.PZ - tPosition->tPositionPriceBaseReaded.PZ) > 0.1) {
            ss << tPosition->Caption; ss << std::endl;
            ss << "PZ calculated isn't equal PZ readed...";
            ss << tPosition->tPositionPriceBaseCalculated.PZ;
            ss << " != ";
            ss << tPosition->tPositionPriceBaseReaded.PZ;
            std::runtime_error ex(ss.str());
            throw ex;
        }
    }
    catch (std::runtime_error& ex) {
        std::cout << ex.what();
        return true;
    }
    return false;
}

void TinyClassObj::calculatePlanNacl(PositionOBJ* tPosition, const std::vector<vid_Rab>& tVid_Rab, double& OZ, double& ZM, double& koefOZ,double& koefZM) // oz zm
{
    for (auto x : tVid_Rab) {

        if (x.id == tPosition ->Vr2001&& !x.calculated) {
            tPosition->tPositionCalculatedVidRab.id = x.id;
            double tmpOZ;
            tmpOZ =OZ;
            tmpOZ=floor(tmpOZ * 100 + 0.5) /100;
            tmpOZ *= tPosition->Quantity;
            tmpOZ=floor(tmpOZ * 100 + 0.5) /100;

            double tmpZM;
            tmpZM =ZM;
            tmpZM=floor(tmpZM * 100 + 0.5) /100;
            tmpZM *= tPosition->Quantity;
            tmpZM=floor(tmpZM * 100 + 0.5) /100;

            double result;
            result =tmpZM+ tmpOZ;

            tPosition->tPositionCalculatedVidRab.Nacl=result * x.Nacl / 100;
            tPosition->tPositionCalculatedVidRab.Nacl=floor( tPosition->tPositionCalculatedVidRab.Nacl*100 +0.5)/100;

            tPosition->tPositionCalculatedVidRab.Plan=result * x.Plan / 100;
            tPosition->tPositionCalculatedVidRab.Plan=floor( tPosition->tPositionCalculatedVidRab.Plan*100 +0.5)/100;

            tPosition->tPositionCalculatedVidRab.calculated = true;
            x.calculated = true;
            break;
        }
    }
}
