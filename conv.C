/*
 * conv.C - root macro
 * Author K. Mizukoshi
 * DRS4 binary data test
 * IEEE real time school 2018 Cape Town
 * 2018 Jul 13
 * Ref. https://www.psi.ch/drs/DocumentationEN/manual_rev50.pdf
*/


int conv(TString filename = "test10ev.dat"){

    // data file open
    ifstream file;
    file.open(filename, ios::in | ios::binary);
    if(file.is_open()){
        cout << "file open " << filename << endl;
    }else{
        cerr << "file open error" << endl;
        return 1;
    }


    // Char_t   1 byte
    // Short_t  2 byte
    // Int_t    4 byte

    // variable for header
    Char_t fileHeader[4];
    Char_t timeHeader[4];
    Char_t boardSerialId[2];
    Short_t boardSerial;
    Char_t ch1Header[4];
    Char_t ch2Header[4];
    Float_t time1BinWidth[1024];
    Float_t time2BinWidth[1024];

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

    auto tree = new TTree("tree","DRS4 data");
    tree->Branch("time1BinWidth",time1BinWidth,"time1BinWidth[1024]/F");
    tree->Branch("time2BinWidth",time2BinWidth,"time2BinWidth[1024]/F");
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

    // Read Header
    file.read((char*) &fileHeader, 4);
    file.read((char*) &timeHeader, 4);
    file.read((char*) &boardSerialId, 2);
    file.read((char*) &boardSerial, 2);
    file.read((char*) &ch1Header, 4);
    file.read((char*) &time1BinWidth, 4096);
    file.read((char*) &ch2Header, 4);
    file.read((char*) &time2BinWidth, 4096);


    //Read Events
    while(file.read((char*) &eventHeader, 4)){

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
        file.read((char*) &ch1EventHeader, 4);
        file.read((char*) &ch1Scaler, 4);
        file.read((char*) &ch1wf, 2048);
        file.read((char*) &ch2EventHeader, 4);
        file.read((char*) &ch2Scaler, 4);
        file.read((char*) &ch2wf, 2048);

        tree->Fill();
    }
    auto fout = new TFile("out.root","recreate");
    tree->Write();
    fout->Close();


    return 0;
}
