// -*- C++ -*-
//
// Package:    inputHepMC/inputHepMC
// Class:      inputHepMC
// 
/**\class inputHepMC inputHepMC.cc inputHepMC/inputHepMC/plugins/inputHepMC.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
*/
//
// Original Author:  Mauricio Thiel
//         Created:  Thu, 07 Apr 2016 02:22:36 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/Handle.h"

#include <iostream>

//
// class declaration
//

class inputHepMC : public edm::stream::EDFilter<> {
	public:
		explicit inputHepMC(const edm::ParameterSet&);
		~inputHepMC();

		static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

	private:
		virtual void beginStream(edm::StreamID) override;
		virtual bool filter(edm::Event&, const edm::EventSetup&) override;
		virtual void endStream() override;
		edm::EDGetToken token_;

		int nM, nE, nQ;

		//virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
		//virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
		//virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
		//virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

		// ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
inputHepMC::inputHepMC(const edm::ParameterSet& iConfig):
	//token_(consumes<edm::HepMCProduct>(edm::InputTag(iConfig.getUntrackedParameter("moduleLabel",std::string("source")),"generator")))
	token_(consumes<edm::HepMCProduct>(edm::InputTag(iConfig.getUntrackedParameter("moduleLabel",std::string("source")),"")))
	, nM (iConfig.getParameter<int>("nM"))
	, nE (iConfig.getParameter<int>("nE"))
	, nQ (iConfig.getParameter<int>("nQ"))
{
	//now do what ever initialization is needed
	//  token_ = consumes<edm::HepMCProduct>( iConfig.getParameter<InputTag>(HepMCProduct) );
}


inputHepMC::~inputHepMC()
{

	// do anything here that needs to be done at destruction time
	// (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
	bool
inputHepMC::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	using namespace edm;
	using namespace std;
	int nLepsM = 0;
	int nLepsE = 0;
	int nQuarks = 0;
	Handle< HepMCProduct > evt;
	iEvent.getByToken(token_, evt);
	const HepMC::GenEvent * myGenEvent = evt->GetEvent();

	for(HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ){
		int pId = abs((*p)->pdg_id());

		if ( pId == 24 || pId == 23 ){ //15
			for ( HepMC::GenVertex::particle_iterator dau  =(*p)->end_vertex()->particles_begin(HepMC::children); dau != (*p)->end_vertex()->particles_end(HepMC::children); ++dau ) {
				if ( abs((*dau)->pdg_id()) == 24 || abs((*dau)->pdg_id()) == 23){ //15
					for ( HepMC::GenVertex::particle_iterator ddau  =(*dau)->end_vertex()->particles_begin(HepMC::children); ddau != (*dau)->end_vertex()->particles_end(HepMC::children); ++ddau ) {
						if (abs((*ddau)->pdg_id()) == 15 ){
							for ( HepMC::GenVertex::particle_iterator dddau  =(*ddau)->end_vertex()->particles_begin(HepMC::children); dddau != (*ddau)->end_vertex()->particles_end(HepMC::children); ++dddau ) {
								if(abs((*dddau)->pdg_id()) == 13){
									nLepsM++;
								}
								if(abs((*dddau)->pdg_id()) == 11){
									nLepsE++;
								}
							}
						}
						if (abs((*ddau)->pdg_id()) == 13 ){
							nLepsM++;}
						if (abs((*ddau)->pdg_id()) == 11 ){
							nLepsE++;}
						if (abs((*ddau)->pdg_id()) == 1 ){nQuarks++;}
						if (abs((*ddau)->pdg_id()) == 2 ){nQuarks++;}
						if (abs((*ddau)->pdg_id()) == 3 ){nQuarks++;}
						if (abs((*ddau)->pdg_id()) == 4 ){nQuarks++;}
						if (abs((*ddau)->pdg_id()) == 5 ){nQuarks++;}
						if (abs((*ddau)->pdg_id()) == 6 ){nQuarks++;}
					}
				}

			}

		}
	}

	if (nLepsM == nM && nQuarks == nQ && nLepsE == nE){
		//   cout << "We have an event!! :)" << endl;
		return true;
	} else {
		//   cout << "        We don't have event!! :(" << endl;
		return false;
	}
	return true;


#ifdef THIS_IS_AN_EVENT_EXAMPLE
	Handle<ExampleData> pIn;
	iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
	ESHandle<SetupData> pSetup;
	iSetup.get<SetupRecord>().get(pSetup);
#endif
}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
	void
inputHepMC::beginStream(edm::StreamID)
{
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void
inputHepMC::endStream() {
}

// ------------ method called when starting to processes a run  ------------
/*
   void
   inputHepMC::beginRun(edm::Run const&, edm::EventSetup const&)
   { 
   }
   */

// ------------ method called when ending the processing of a run  ------------
/*
   void
   inputHepMC::endRun(edm::Run const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when starting to processes a luminosity block  ------------
/*
   void
   inputHepMC::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method called when ending the processing of a luminosity block  ------------
/*
   void
   inputHepMC::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
   {
   }
   */

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
inputHepMC::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(inputHepMC);
