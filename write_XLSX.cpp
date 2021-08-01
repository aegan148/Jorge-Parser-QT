#include "write_XLSX.h"
#include <cmath>

QString file("D:\\qt_proj\\build-hello-Desktop_Qt_5_13_1_MinGW_32_bit-Debug\\ReadFrom\\Pattern.xlsx");


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
            tAttribute.podRazdel = x.Podrazdel;
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
                tAttribute.itogo = tAttribute.matPrice + tAttribute.rabPrice;
                tAttribute.itogoNds = tAttribute.matPriceNds + tAttribute.rabPriceNds;
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
    int col=5;
    int num_Glava=1;
    int razdelRow=0;
    int podRazdelRow=0;
    int podRazdelCount=1;

    bool firstRazdel=true;
    std::string ss;

    QAxObject* excel = new QAxObject("Excel.Application",nullptr);
    QAxObject* workbooks= excel->querySubObject("Workbooks");
    QAxObject* workbook= workbooks->querySubObject("Open(const QString&)",file);
    excel->dynamicCall("SetVisible(bool)",false);
    QAxObject* worksheet= workbook->querySubObject("WorkSheets(int)",1);
    QAxObject* cell;
    QAxObject* font;
    for (const auto& a : this->glavi) {
        col=5;
        razdelRow = row;
        ss= "Раздел " + std::to_string(num_Glava) +". " + a.nameOfRazdel;
        cell= worksheet->querySubObject("Cells(int,int)",row,col);
        cell->dynamicCall("SetValue(const QVariant&)",ss.c_str());

        ss="Range(A"+std::to_string(row)+",O"+std::to_string(row)+")";
        cell= worksheet->querySubObject( ss.c_str());

        font=cell->querySubObject("Interior");
        font->setProperty("Color",QColor(245, 245, 220));

        num_Glava++;
        row++;
        podRazdelCount=1;
        firstRazdel=true;
        for (const auto& x : a.atribute) {
            if(!x.podRazdel.empty()){
                col=5;
                podRazdelRow=row;
                cell= worksheet->querySubObject("Cells(int,int)",row,col);
                cell->dynamicCall("SetValue(const QVariant&)",x.podRazdel.c_str());
                                                //"Range(A1,G123)"
                ss="Range(A"+std::to_string(row)+",O"+std::to_string(row)+")";
                cell= worksheet->querySubObject( ss.c_str());

                font=cell->querySubObject("Interior");
                font->setProperty("Color",QColor(255, 255, 240));

                row++;
                firstRazdel=false;
            } else if (firstRazdel){
                col=5;
                podRazdelRow=row;
                ss = "Подраздел - " + a.nameOfRazdel;
                cell= worksheet->querySubObject("Cells(int,int)",row,col);
                cell->dynamicCall("SetValue(const QVariant&)",ss.c_str());
                ss="Range(A"+std::to_string(row)+",O"+std::to_string(row)+")";
                cell= worksheet->querySubObject( ss.c_str());
                font=cell->querySubObject("Interior");
                font->setProperty("Color",QColor(255, 255, 240));
                row++;
                firstRazdel=false;
            }
            col=1;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",podRazdelCount);
            podRazdelCount++;
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.noLSR.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.noFer.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.noPos);
            col=5;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.namePos.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.edIzm.c_str());
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.kolvo);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.matPrice);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.matPriceNds);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.rabPrice);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.rabPriceNds);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",0);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",0);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.itogo);
            col++;
            cell= worksheet->querySubObject("Cells(int,int)",row,col);
            cell->dynamicCall("SetValue(const QVariant&)",x.itogoNds);
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
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators("D:\\qt_proj\\build-hello-Desktop_Qt_5_13_1_MinGW_32_bit-Debug\\result\\res.xlsx"));
    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
}
