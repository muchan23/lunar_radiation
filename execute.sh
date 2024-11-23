
#!/bin/bash

# 粒子種とエネルギーのリスト
particles=("proton" "alpha" "e-" "gamma")
energies=(1 10 100) # GeV単位のエネルギーリスト

for particle in "${particles[@]}"
do
    for energy in "${energies[@]}"
    do
        # run.macファイルを生成
        cat <<EOL > run_temp.mac
/run/numberOfThreads 8
/run/initialize
/gun/momentumAmp ${energy} GeV
/gun/position 0 0 -30000 m
/gun/particle ${particle}
/run/beamOn 10
EOL

        # Geant4実行ファイルを実行
        ./sim run_temp.mac

        # ROOTマクロを実行して結果を可視化
        root -l -b -q '../plot_flux.C'
    done
done