/////////////////////////////////////////////////////////////////////////////
// Name:        tuplet.cpp
// Author:      Rodolfo Zitellini
// Created:     26/06/2012
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "tuplet.h"

//----------------------------------------------------------------------------

#include "assert.h"

//----------------------------------------------------------------------------

#include "beam.h"
#include "chord.h"
#include "editorial.h"
#include "note.h"
#include "rest.h"
#include "vrv.h"

namespace vrv {

//----------------------------------------------------------------------------
// Tuplet
//----------------------------------------------------------------------------

Tuplet::Tuplet()
    : LayerElement("tuplet-"), ObjectListInterface(), AttDurationRatio(), AttNumberplacement(), AttTupletVis()
{
    RegisterAttClass(ATT_DURATIONRATIO);
    RegisterAttClass(ATT_NUMBERPLACEMENT);
    RegisterAttClass(ATT_TUPLETVIS);

    Reset();
}

Tuplet::~Tuplet()
{
}

void Tuplet::Reset()
{
    LayerElement::Reset();
    ResetDurationRatio();
    ResetNumberplacement();
    ResetTupletVis();
}

void Tuplet::AddChild(Object *child)
{
    if (child->Is() == BEAM) {
        assert(dynamic_cast<Beam *>(child));
    }
    else if (child->Is() == CHORD) {
        assert(dynamic_cast<Chord *>(child));
    }
    else if (child->Is() == CLEF) {
        assert(dynamic_cast<Clef *>(child));
    }
    else if (child->Is() == NOTE) {
        assert(dynamic_cast<Note *>(child));
    }
    else if (child->Is() == REST) {
        assert(dynamic_cast<Rest *>(child));
    }
    else if (child->Is() == TUPLET) {
        assert(dynamic_cast<Tuplet *>(child));
    }
    else if (child->IsEditorialElement()) {
        assert(dynamic_cast<EditorialElement *>(child));
    }
    else {
        LogError("Adding '%s' to a '%s'", child->GetClassName().c_str(), this->GetClassName().c_str());
        assert(false);
    }

    child->SetParent(this);
    m_children.push_back(child);
    Modify();
}

void Tuplet::FilterList(ListOfObjects *childList)
{
    // We want to keep only notes and rests
    // Eventually, we also need to filter out grace notes properly (e.g., with sub-beams)
    ListOfObjects::iterator iter = childList->begin();

    while (iter != childList->end()) {
        if (!(*iter)->IsLayerElement() || !(*iter)->HasInterface(INTERFACE_DURATION)) {
            iter = childList->erase(iter);
        }
        else {
            iter++;
        }
    }
}

} // namespace vrv
