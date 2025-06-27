void ExtractLast3HexDigitsToDecimal() {
    TFile *infile = TFile::Open("output.root");

    TTree *intree = (TTree*)infile->Get("VME_QDC_Raw_data");
    if (!intree) {
        std::cerr << "Error: TTree 'VME_QDC_Raw_data' not found!" << std::endl;
        return;
    }

    UInt_t qdc_val;
    intree->SetBranchAddress("QDC_Value", &qdc_val);
    Long64_t nentries = intree->GetEntries();

    // Newfile for last 3 digits
    TFile *outfile = new TFile("last3hex_processed.root", "RECREATE");
    TTree *outtree = new TTree("T", "Decimal from last 3 hex digits");

    UInt_t last3hex_decimal;
    outtree->Branch("Last3Hex_Decimal", &last3hex_decimal, "Last3Hex_Decimal/i");

    for (Long64_t i = 0; i < nentries; ++i) {
        intree->GetEntry(i);

        last3hex_decimal = qdc_val & 0xFFF;  // Masking lower 12 bits

        std::cout << "Entry " << i
                  << " | QDC_Value: 0x" << std::hex << std::setw(8) << std::setfill('0') << qdc_val
                  << " | Last 3 Hex: " << std::hex << (qdc_val & 0xFFF)
                  << " | Decimal: " << std::dec << last3hex_decimal << std::endl;

        outtree->Fill();
    }

    outtree->Write();
    outfile->Close();
    infile->Close();

    //std::cout << "Done! Output saved in 'last3hex_processed.root'" << std::endl;
}

