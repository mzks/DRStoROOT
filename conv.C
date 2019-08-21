/*
 * conv.C - root macro
 * Author K. Mizukoshi
 * DRS4 binary data test
 * IEEE real time school 2018 Cape Town
 * 2018 Jul 13
 * Ref. https://www.psi.ch/drs/DocumentationEN/manual_rev50.pdf
*/


int conv(TString inFilename = "test10ev.dat", TString outFileName = "out.root"){

    // data file open
    ifstream file;
    file.open(inFilename, ios::in | ios::binary);
    if(file.is_open()){
        cout << "file open " << inFilename << endl;
    }else{
        cerr << "file open error" << endl;
        return 1;
    }

    // Char_t   1 byte
    // Short_t  2 byte
    // Int_t    4 byte

    Char_t  buf1b;
    Short_t buf2b;
    Int_t   buf4b;

    Char_t header[4];


    // variable for header
    Char_t fileHeader[4];
    Char_t timeHeader[4];
    Char_t boardSerialId[2];
    Short_t boardSerial;
    Char_t ch1Header[4];
    Char_t ch2Header[4];
    Float_t time1BinWidth[1024];
    Float_t time2BinWidth[1024];
    Float_t time3BinWidth[1024];
    Float_t time4BinWidth[1024];

    // variable for event
    Char_t eventHeader[4];
    Int_t   eventSerial;
    Short_t Year;
    Short_t Month;
    Short_t Day;
    Short_t Hour;
    Short_t Minute;
    Short_t Second;
    Short_t MillSecond;
    Short_t Range;

    Char_t boardSerialId2[2];
    Short_t boardSerial2;
    Char_t triggerSellId[2];
    Short_t triggerSell;
    Char_t  ch1EventHeader[4];
    Int_t   ch1Scaler;
    Short_t ch1wf[1024];
    Char_t  ch2EventHeader[4];
    Int_t   ch2Scaler;
    Short_t ch2wf[1024];
    Char_t  ch3EventHeader[4];
    Int_t   ch3Scaler;
    Short_t ch3wf[1024];
    Char_t  ch4EventHeader[4];
    Int_t   ch4Scaler;
    Short_t ch4wf[1024];

    auto fout = new TFile("out.root","recreate");
    auto tree = new TTree("tree","DRS4 data");
    //tree->Branch("time1BinWidth",time1BinWidth,"time1BinWidth[1024]/F");
    //tree->Branch("time2BinWidth",time2BinWidth,"time2BinWidth[1024]/F");

    tree->Branch("eventSerial",&eventSerial,"eventSerial/I");
    tree->Branch("Year",&Year,"Year/S");
    tree->Branch("Month",&Month,"Month/S");
    tree->Branch("Day",&Day,"Day/S");
    tree->Branch("Hour",&Hour,"Hour/S"); 
    tree->Branch("Minute",&Minute,"Minute/S");
    tree->Branch("Second",&Second,"Second/S");
    tree->Branch("MillSecond",&MillSecond,"MillSecond/S");
    tree->Branch("Range",&Range,"Range/S");
    tree->Branch("triggerSell",&triggerSell,"triggerSell/S");
    tree->Branch("ch1wf",ch1wf,"ch1wf[1024]/S");
    tree->Branch("ch2wf",ch2wf,"ch2wf[1024]/S");
    tree->Branch("ch1Scaler",&ch1Scaler,"ch1Scaler/I");
    tree->Branch("ch2Scaler",&ch2Scaler,"ch2Scaler/I");


    //Read Events

    ULong64_t ievents = 0;

    file.read((char*) &header, 4);
    while(true){
        if(ievents % 100 == 0) cout << ievents << endl;

        if(header[0]=='D' && header[1]=='R' && header[2]=='S'){
            cout << "Welcome, the DRS Version is " << header[3] << endl;
            if(!file.read((char*) &header, 4)) break;
        }

        if(header[0]=='T' && header[1]=='I' && header[2]=='M' && header[3]=='E'){

            file.read((char*) &header, 2); // 'B#'
            file.read((char*) &boardSerial, 2); // Serial Number
            cout << "Serial number is " << boardSerial << endl;
            if(!file.read((char*) &header, 4)) break;

            while(true){
                if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='1'){
                    cout << "ch1" << endl;
                    file.read((char*) &time1BinWidth, 4096);
                } else if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='2'){
                    cout << "ch2" << endl;
                    file.read((char*) &time2BinWidth, 4096);
                } else if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='3'){
                    cout << "ch3" << endl;
                    file.read((char*) &time3BinWidth, 4096);
                } else if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='4'){
                    cout << "ch4" << endl;
                    file.read((char*) &time4BinWidth, 4096);
                } else {
                    //if(!file.read((char*) &header, 4)) break;
                    break;
                }
                if(!file.read((char*) &header, 4)) break;
            }
        }


        if(header[0]=='E' && header[1]=='H' && header[2]=='D' && header[3]=='R'){
            
            ++ievents;

            file.read((char*) &eventSerial, 4);
            file.read((char*) &Year, 2);
            file.read((char*) &Month, 2);
            file.read((char*) &Day, 2);
            file.read((char*) &Hour, 2);
            file.read((char*) &Minute, 2);
            file.read((char*) &Second, 2);
            file.read((char*) &MillSecond, 2);
            file.read((char*) &Range, 2);

            file.read((char*) &boardSerialId2, 2);
            file.read((char*) &boardSerial2, 2);
            file.read((char*) &triggerSellId, 2);
            file.read((char*) &triggerSell, 2);

            if(!file.read((char*) &header, 4)) break;
            while(true){
                if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='1'){
                    file.read((char*) &ch1Scaler, 4);
                    file.read((char*) &ch1wf, 2048);
                } else if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='2'){
                    file.read((char*) &ch2Scaler, 4);
                    file.read((char*) &ch2wf, 2048);
                } else if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='3'){
                    file.read((char*) &ch3Scaler, 4);
                    file.read((char*) &ch3wf, 2048);
                } else if(header[0]=='C' && header[1]=='0' && header[2]=='0' && header[3]=='4'){
                    file.read((char*) &ch4Scaler, 4);
                    file.read((char*) &ch4wf, 2048);
                } else {
                    tree->Fill();
                    break;
                }
                if(!file.read((char*) &header, 4)){
                    goto fileover;
                }
            }
        }
    }

    fileover:
    tree->Write();
    fout->Close();

    return 0;
}
