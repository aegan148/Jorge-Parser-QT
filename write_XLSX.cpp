#include "write_XLSX.h"
#include <cmath>

QString file("D:\\qt_proj\\build-hello-Desktop_Qt_5_13_1_MinGW_32_bit-Debug\\ReadFrom\\traf.xlsx");


void doc_Razdel::calculateData(const std::vector<chapterOBJ>& gChaperList,const std::string& propertysOfDocument, const double& smr){

    bool recalculate=false;
    doc_Attribute temp_tAttribute;

    for (const auto& a : gChaperList) {
        doc_PodRazdel tPodRazdel;
        tPodRazdel.nameOfRazdel= a.chapterName;
        for (const auto& x : a.tPosition) {
            if(x.Dont_add) continue;
            if(x.number == 60){
                std::cout<<"df";
            }
            doc_Attribute tAttribute;
            tAttribute.namePos= x.Caption;
            tAttribute.edIzm= x.Units;
            tAttribute.unit_kolvo= x.KUnit;
            tAttribute.kolvo=x.Quantity * x.KUnit;
            tAttribute.noFer=x.Code;
            tAttribute.noLSR= propertysOfDocument;
            tAttribute.noPos =x.number;
            tAttribute.matPriceNds = x.matPriceNdsCalculated *smr;
            tAttribute.matPriceNds =floor( tAttribute.matPriceNds*100 + 0.5)/100.0;
            tAttribute.matPrice = tAttribute.matPriceNds / tAttribute.kolvo;
            double tMt, tZm, tOz, tPz, tEm;
            tMt= x.tPositionPriceBaseCalculated.MT;
            tZm= x.tPositionPriceBaseCalculated.ZM;
            tOz= x.tPositionPriceBaseCalculated.OZ;
            tOz=floor( tOz*100 + 0.5)/100.0;
            tPz= x.tPositionPriceBaseCalculated.PZ;
            tEm= x.tPositionPriceBaseCalculated.EM;
            tEm=floor( tEm*100 + 0.5)/100.0;
            tOz *= x.Quantity;
            tOz=floor( tOz*100 + 0.5)/100.0;
            tEm *= x.Quantity;
            tEm = floor(tEm *100 +0.5) /100.0;

            tAttribute.rabPriceNds =( (tOz + tEm) + x.tPositionCalculatedVidRab.Nacl+ x.tPositionCalculatedVidRab.Plan);
            tAttribute.rabPriceNds = int(tAttribute.rabPriceNds*100 +0.5)/100.0;
            tAttribute.rabPriceNds *= smr;
            tAttribute.rabPriceNds = int(tAttribute.rabPriceNds*100 +0.5)/100.0;
            tAttribute.rabPrice= tAttribute.rabPriceNds / tAttribute.kolvo;

            if(recalculate){
                tAttribute.noFer = temp_tAttribute.noFer;
                tAttribute.noLSR = temp_tAttribute.noLSR;
                tAttribute.namePos = temp_tAttribute.namePos;
                tAttribute.noPos = temp_tAttribute.noPos;
                tAttribute.matPrice += temp_tAttribute.matPrice;
                tAttribute.matPriceNds += temp_tAttribute.matPriceNds;
                tAttribute.rabPrice += temp_tAttribute.rabPrice;
                tAttribute.rabPriceNds += temp_tAttribute.rabPriceNds;
            }

            if(x.koef_each){
                temp_tAttribute = tAttribute;
                recalculate =true;
            }
            else {
                tPodRazdel.atribute.push_back(tAttribute);
                recalculate =false;
            }
        }
        this->glavi.push_back(tPodRazdel);
    }
}

void doc_Razdel::GomakeXlsx(const std::vector<chapterOBJ>& gChaperList, const std::vector<vid_Rab>& tVid_Rab, const indexes& koefList, const double SMR,const std::string& propertysOfDocument)
{

    calculateData(gChaperList, propertysOfDocument, SMR);
    int row=7;
    int col=1;
    QAxObject* excel = new QAxObject("Excel.Application",nullptr);
    QAxObject* workbooks= excel->querySubObject("Workbooks");
    QAxObject* workbook= workbooks->querySubObject("Open(const QString&)",file);
    excel->dynamicCall("SetVisible(bool)",false);
    QAxObject* worksheet= workbook->querySubObject("WorkSheets(int)",1);
    QAxObject* cell;
    for (const auto& a : this->glavi) {
        for (const auto& x : a.atribute) {
            col=1;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",1);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.noLSR.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.noFer.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",2);
            col+=4;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.namePos.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.edIzm.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.kolvo);
            row++;
        }
    }
    // опять же,указатель на D2
    //QAxObject *rangec = worksheet->querySubObject( "Range(const QVariant&)",QVariant(QString("A16:A324")));
    // получаю указатель на строку
   // QAxObject *razmer = rangec->querySubObject("Rows");
    //rangec->dynamicCall("Select()");
    // устанавливаю её размер.
    //rangec->setProperty("RowHeight",20);
    //rangec->setProperty("RowWidth",10);
    workbook->dynamicCall("Save()");
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
}
