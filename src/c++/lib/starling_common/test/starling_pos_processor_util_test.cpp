// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Strelka - Small Variant Caller
// Copyright (c) 2009-2016 Illumina, Inc.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//

#include "boost/test/unit_test.hpp"

#include "starling_pos_processor_util.hh"


BOOST_AUTO_TEST_SUITE( sppr_util_test )

BOOST_AUTO_TEST_CASE( test_vcf_to_allele )
{
    static const unsigned max_indel_size(50);
    IndelObservation obs;
    bool isConverted;

    {
        // "chr20 4329513 . ATT AT,A 9114 PASS LEN=1,2;TYPE=del,del GT 2|1"
        vcf_record vr;
        vr.chrom = "chr20";
        vr.pos = 4329513;
        vr.ref = "ATT";
        vr.alt.push_back("AT");
        vr.alt.push_back("A");

        isConverted = convert_vcfrecord_to_indel_allele(max_indel_size,vr,0,obs);
        indel_key k0expect(4329513,INDEL::DELETE,1);
        BOOST_REQUIRE(isConverted);
        BOOST_REQUIRE_EQUAL(obs.key,k0expect);

        isConverted = convert_vcfrecord_to_indel_allele(max_indel_size,vr,1,obs);
        indel_key k1expect(4329513,INDEL::DELETE,2);
        BOOST_REQUIRE(isConverted);
        BOOST_REQUIRE_EQUAL(obs.key,k1expect);
    }

    {
        // "chr20 4329513 . ATT A,ATTT,AG"
        vcf_record vr;
        vr.chrom = "chr20";
        vr.pos = 4329513;
        vr.ref = "ATT";
        vr.alt.push_back("A");
        vr.alt.push_back("ATTT");
        vr.alt.push_back("AG");

        isConverted = convert_vcfrecord_to_indel_allele(max_indel_size,vr,0,obs);
        indel_key k0expect(4329513,INDEL::DELETE,2);
        BOOST_REQUIRE(isConverted);
        BOOST_REQUIRE_EQUAL(obs.key,k0expect);

        isConverted = convert_vcfrecord_to_indel_allele(max_indel_size,vr,1,obs);
        indel_key k1expect(4329513,INDEL::INSERT,1);
        BOOST_REQUIRE(isConverted);
        BOOST_REQUIRE_EQUAL(obs.key,k1expect);

        isConverted = convert_vcfrecord_to_indel_allele(max_indel_size,vr,2,obs);
        indel_key k2expect(4329513,INDEL::SWAP,1,2);
        BOOST_REQUIRE(isConverted);
        BOOST_REQUIRE_EQUAL(obs.key,k2expect);
    }
}

BOOST_AUTO_TEST_SUITE_END()


