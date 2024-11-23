// シミュレーションの初期化、実行、終了を行う

#include <iostream>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "QGSP_BERT.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* ui = 0;

    #ifdef G4MULTITHREADED
      G4MTRunManager* runManager = new G4MTRunManager;
    #else
      G4RunManager* runManager = new G4RunManager;
    #endif

    runManager->SetUserInitialization(new MyDetectorConstruction()); //検出器の構築
    runManager->SetUserInitialization(new MyPhysicsList()); //物理リストの設定
    runManager->SetUserInitialization(new MyActionInitialization()); // アクションの設定

    G4VModularPhysicsList* physics = new QGSP_BERT(); // 物理リストの設定
    physics->RegisterPhysics(new G4DecayPhysics());
    runManager->SetUserInitialization(physics); //物理リストをrunManagerに設定

    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv); //
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if(ui)
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    return 0;
}