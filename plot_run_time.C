void plot_run_time()
{
    TFile* file = TFile::Open("/Users/murakamitomoki/sim4/build/output0_t0.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    TTree* tree = (TTree*)file->Get("RunTime");
    if (!tree) {
        std::cerr << "Tree not found" << std::endl;
        return;
    }

    TCanvas* c1 = new TCanvas("c1", "Run Time Histogram", 800, 600);
    tree->Draw("Time", "", "hist");

    // ヒストグラムをPNGファイルとして保存
    c1->SaveAs("run_time_histogram.png");
}