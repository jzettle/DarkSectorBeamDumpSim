void view_geo()
{
  TGeoManager::Import("simple.gdml");
  gGeoManager->GetTopNode()->Draw("ogl");
}
