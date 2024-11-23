void plot_flux()
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

    // 検出器の面積（平方メートル）
    double detector_area = 250.0; // 例として250平方メートルとする

    // ヒストグラムを作成（横軸の範囲を0から350に設定）
    TH1D *hFlux = new TH1D("hFlux", "Flux Distribution", 1000, 0.1, 100000.0);
    tree->Draw("fEnergy>>hFlux");
    std::cout << "Histogram created successfully." << std::endl;

    // フラックスを計算
    double bin_width = hFlux->GetXaxis()->GetBinWidth(1); // エネルギーのビン幅を取得
    hFlux->Scale(1.0 / (detector_area * bin_width)); // 面積とビン幅でスケーリング

    // 軸ラベルを設定
    hFlux->GetXaxis()->SetTitle("Energy (MeV)");
    hFlux->GetYaxis()->SetTitle("Flux (particles/m^2/MeV)");

    // 縦軸の範囲を指定
    hFlux->SetMinimum(1e-8); // 縦軸の最小値を設定
    hFlux->SetMaximum(1e2);  // 縦軸の最大値を設定

    // フィット関数を定義（例としてガウス関数を使用）
    TF1 *fitFunc = new TF1("fitFunc", "gaus", 0.1, 100000.0);
    // TF1 *fitFunc = new TF1("fitFunc", "expo", 0.1, 50000.0);
    // TF1 *fitFunc = new TF1("fitFunc", "pol3", 0.1, 50000.0);
    // TF1 *fitFunc = new TF1("fitFunc", "lorentz", 0.1, 50000.0);

    // ヒストグラムにフィットを適用
    hFlux->Fit(fitFunc);
    std::cout << "Fitting completed successfully." << std::endl;

    // ヒストグラムとフィット結果を描画
    TCanvas *c1 = new TCanvas("c1", "Flux Distribution", 800, 600);
    c1->SetLogx(); // x軸を対数スケールに設定
    c1->SetLogy(); // y軸を対数スケールに設定
    hFlux->Draw();
    fitFunc->Draw("same");
    std::cout << "Histogram and fit drawn successfully." << std::endl;

    // ヒストグラムをPNGファイルとして保存
    c1->SaveAs("/Users/murakamitomoki/sim4/build/flux_distribution.png");
    std::cout << "Histogram saved as flux_distribution.png" << std::endl;

    // ファイルを閉じる
    file->Close();
    std::cout << "File closed successfully." << std::endl;
}