void plot_flux_line()
{
    // ROOTファイルを開く
    TFile *file = TFile::Open("/Users/murakamitomoki/sim4/build/output0_t0.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }
    std::cout << "File opened successfully." << std::endl;

    // TTreeを取得
    TTree *tree = (TTree*)file->Get("Photons");
    if (!tree) {
        std::cerr << "Error getting TTree" << std::endl;
        return;
    }
    std::cout << "TTree obtained successfully." << std::endl;

    // エネルギーのデータを取得
    double energy;
    tree->SetBranchAddress("fEnergy", &energy);

    // データを読み込む
    Long64_t nEntries = tree->GetEntries();
    std::vector<double> energies(nEntries);
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        energies[i] = energy;
    }

    // 検出器の面積（平方メートル）
    double detector_area = 250.0; // 例として250平方メートルとする

    // ヒストグラムを作成（横軸の範囲を0から350に設定）
    TH1D *hFlux = new TH1D("hFlux", "Flux Distribution", 1000, 0.001, 100000.0);
    for (double e : energies) {
        hFlux->Fill(e);
    }
    std::cout << "Histogram created successfully." << std::endl;

    // フラックスを計算
    double bin_width = hFlux->GetXaxis()->GetBinWidth(1); // エネルギーのビン幅を取得
    hFlux->Scale(1.0 / (detector_area * bin_width)); // 面積とビン幅でスケーリング

    // TGraphを作成
    TGraph *graph = new TGraph();
    for (int i = 1; i <= hFlux->GetNbinsX(); ++i) {
        double bin_center = hFlux->GetBinCenter(i);
        double flux = hFlux->GetBinContent(i);
        graph->SetPoint(i - 1, bin_center, flux);
    }

    // 軸ラベルを設定
    graph->GetXaxis()->SetTitle("Energy (MeV)");
    graph->GetYaxis()->SetTitle("Flux (particles/m^2/MeV)");

    // 縦軸の範囲を設定
    graph->SetMinimum(1e-8); // 縦軸の最小値を設定
    graph->SetMaximum(1e2);  // 縦軸の最大値を設定

    // グラフを描画
    TCanvas *c1 = new TCanvas("c1", "Flux Distribution", 800, 600);
    c1->SetLogx(); // x軸を対数スケールに設定
    c1->SetLogy(); // y軸を対数スケールに設定
    graph->Draw("A*"); // 点を線で結ぶように描画
    // x軸の範囲を設定
    // graph->GetXaxis()->SetLimits(0.001, 100000.0); // x軸の範囲を設定
    std::cout << "Graph drawn successfully." << std::endl;

    // グラフをPNGファイルとして保存
    c1->SaveAs("/Users/murakamitomoki/sim4/build/flux_distribution.png");
    std::cout << "Graph saved as flux_distribution.png" << std::endl;

    // ファイルを閉じる
    file->Close();
    std::cout << "File closed successfully." << std::endl;
}