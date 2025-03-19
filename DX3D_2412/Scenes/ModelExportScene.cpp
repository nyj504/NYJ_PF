#include "Framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
    string name = "SteveRigged";
    string extension = "fbx";
    string file = "Resources/Models/FBX/" + name + "." + extension;

    ModelExporter* exporter = new ModelExporter(name, file);
    exporter->ExportMaterial();
    exporter->ExportMesh();
    delete exporter;

    string clipName = "Walk";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";    
    
    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;

    clipName = "BowShoot";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
     clipName = "BowCharging";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
     clipName = "Jump";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
     clipName = "Mining";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
     clipName = "Dance";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
     clipName = "Swimming";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
     clipName = "HandAttack";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);

    delete exporter;

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
     clipName = "Dying";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);
    delete exporter;
    clipName = "Run";
    file = "Resources/Models/Animations/" + name + "/" + clipName + ".fbx";

    exporter = new ModelExporter(name, file);
    exporter->ExportClip(clipName);

    delete exporter;
}

ModelExportScene::~ModelExportScene()
{
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}
