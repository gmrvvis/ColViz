/*
 * Copyright (c) 2017 CCS/GMRV/UPM/URJC.
 *
 * Author: Juan P. Brito <juanpedro.brito@upm.es>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "SWCImporter.h"

#include <iostream>

using namespace std;

#define SOMAID						1
#define MINSOMADESP					0.5

#define AXONRADIUSMODIF				1.0
#define DENDRITICRADIUSMODIF		3.0

#define SOMAAPICALDISTFACTOR		3.0

#define APICALRADIUSMODIFSUDDEN		10.0
#define APICALRADIUSMODIFINITIAL	8.0
#define APICALRADIUSMODIFINFINAL	6.0
#define APICALRADIUSMODIFSTD		4.0

#define BASERADIUSMODIF				0.205

/*
 * Description
 * @author Juan Pedro Brito Mendez
 */
namespace NSUtils
{

  SWCImporter::SWCImporter ( string fileName, bool pApplyStdDims,
      bool pApplyReplicantNodeTest )
  {
    mSomaNodesDiscarded = 0;
    loadFile( fileName, pApplyStdDims, pApplyReplicantNodeTest );
    generateConnectios( );
    generateBifurcations( );
    generateBranches( );
  }

  SWCImporter::~SWCImporter ( )
  {

  }

  void SWCImporter::loadFile ( string fileName, bool pApplyStdDims,
      bool pApplyReplicantNodeTest )
  {
    inputFileTXT.open( fileName.c_str( ), std::ios::in );
    if ( !inputFileTXT )
    {
      cerr << "An error occurred. Unable to read input file." << fileName
          << endl;
      exit( 1 );
    }

    SWCNode nodeAux;

    nodeAux.id = nodeAux.type = nodeAux.position[0] = nodeAux.position[1] =
        nodeAux.position[2] = nodeAux.radius = nodeAux.parent =
            nodeAux.mDendriticDistanceNorm = nodeAux.mDendriticDistanceReal = 0;

    preProcessNodeCollection.push_back( nodeAux );

    char str[2000];
    char coment;
    bool continueReading = true;
    while ( continueReading )
    {
      inputFileTXT.get( coment );
      if ( coment != '#' ) continueReading = false;
      else
      {
        inputFileTXT.getline( str, 2000 );
        description += str;
        description += '\n';
      }
    }

    int pos = 0;
    if ( coment != ' ' )
    {
      ++pos;
      nodeAux.id = 1;
      inputFileTXT >> nodeAux.type;
      inputFileTXT >> nodeAux.position[0];
      inputFileTXT >> nodeAux.position[1];
      inputFileTXT >> nodeAux.position[2];
      inputFileTXT >> nodeAux.radius;
      inputFileTXT >> nodeAux.parent;

      preProcessNodeCollection.push_back( nodeAux );
    }

    while ( !inputFileTXT.eof( ) )
    {
      ++pos;
      inputFileTXT >> nodeAux.id;
      inputFileTXT >> nodeAux.type;
      inputFileTXT >> nodeAux.position[0];
      inputFileTXT >> nodeAux.position[1];
      inputFileTXT >> nodeAux.position[2];
      inputFileTXT >> nodeAux.radius;
      inputFileTXT >> nodeAux.parent;

      preProcessNodeCollection.push_back( nodeAux );
    }

    int tam = preProcessNodeCollection.size( );
    nodeAux = preProcessNodeCollection[tam - 1];
    if ( ( tam - 1 ) != nodeAux.id ) preProcessNodeCollection.pop_back( );

    inputFileTXT.close( );

    SomaThreeCrazyNodesTest( );

    SomaBeltTest( );

    centerSomaInOrigin( );

    SomaDendriticTest( );

    if ( pApplyReplicantNodeTest ) NodeRepetitionTest( );

    calcDendriticDistance( );

    calcDendritics( );

    nodeCollection = preProcessNodeCollection;

    if ( pApplyStdDims ) applyUniformModifiers( AXONRADIUSMODIF,
        DENDRITICRADIUSMODIF, APICALRADIUSMODIFSTD, BASERADIUSMODIF );

    preProcessNodeCollection.clear( );
  }

  void SWCImporter::centerSomaInOrigin ( )
  {
    SWCNode lNodeAux;
    unsigned int lNumNodes = preProcessNodeCollection.size( );
    ;

    lNodeAux = preProcessNodeCollection[1];

    for ( int i = 1; i < lNumNodes; ++i )
    {
      preProcessNodeCollection[i].position[0] -= lNodeAux.position[0];
      preProcessNodeCollection[i].position[1] -= lNodeAux.position[1];
      preProcessNodeCollection[i].position[2] -= lNodeAux.position[2];
    }
  }

  void SWCImporter::SomaDendriticTest ( )
  {
    SWCNode lNodeAux;
    SWCNode lNodeSoma;
    OpenMesh::Vec3f lVecDir;
    float lDespDir = 0;

    unsigned int lNumNodes = preProcessNodeCollection.size( );
    ;

    lNodeSoma = preProcessNodeCollection[1];

    for ( int i = 1; i < lNumNodes; ++i )
    {
      if ( preProcessNodeCollection[i].parent == SOMAID )
      {
        if ( preProcessNodeCollection[i].position.length( ) < lNodeSoma.radius )
        {
          lVecDir = preProcessNodeCollection[i].position - lNodeSoma.position;
          lVecDir.normalize( );

          lDespDir = MINSOMADESP;

          lVecDir *= ( lNodeSoma.radius + lDespDir );

          preProcessNodeCollection[i].position = lVecDir;
        }
      }
    }
  }

  void SWCImporter::SomaThreeCrazyNodesTest ( )
  {
    SWCNode lNodeAux;
    OpenMesh::Vec3f lBariCenter( 0, 0, 0 );
    std::vector<SWCNode> lBeltSomaContainer;
    std::vector<SWCNode> lFinalContainer;

    float lRadUnifor = preProcessNodeCollection[1].radius;
    unsigned int lNumEles = 3;
    unsigned int lNumNodes = preProcessNodeCollection.size( );
    ;
    unsigned int i = 1;

    if ( ( preProcessNodeCollection[1].type == SOMAID )
        && ( preProcessNodeCollection[2].type == SOMAID )
        && ( preProcessNodeCollection[3].type == SOMAID )
        && ( preProcessNodeCollection[4].type != SOMAID )
        && ( lRadUnifor == preProcessNodeCollection[1].radius )
        && ( lRadUnifor == preProcessNodeCollection[2].radius )
        && ( lRadUnifor == preProcessNodeCollection[3].radius )
        && ( preProcessNodeCollection[1].position[0]
            == preProcessNodeCollection[2].position[0] )
        && ( preProcessNodeCollection[1].position[0]
            == preProcessNodeCollection[3].position[0] )
        && ( preProcessNodeCollection[1].position[2]
            == preProcessNodeCollection[2].position[2] )
        && ( preProcessNodeCollection[1].position[2]
            == preProcessNodeCollection[3].position[2] ) )
    {
      for ( int i = 1; i <= 3; i++ )
      {
        lBariCenter += preProcessNodeCollection[i].position;
      }
      lBariCenter = preProcessNodeCollection[1].position;

      lNodeAux = preProcessNodeCollection[1];
      lNodeAux.position = lBariCenter;
      lNodeAux.radius = lRadUnifor;
      lFinalContainer.push_back( preProcessNodeCollection[0] );
      lFinalContainer.push_back( lNodeAux );

      for ( int i = ( lNumEles + 1 ); i < lNumNodes; i++ )
      {
        lNodeAux = preProcessNodeCollection.at( i );

        lNodeAux.id -= ( lNumEles - 1 );

        if ( lNodeAux.type == SOMAID )
        {
          lNodeAux.type = preProcessNodeCollection.at( i + 1 ).type;
          lNodeAux.radius = preProcessNodeCollection.at( i + 1 ).radius;
        }

        if ( lNodeAux.parent <= lNumEles )
        {
          lNodeAux.parent = SOMAID;
        }
        else
        {
          lNodeAux.parent -= ( lNumEles - 1 );
        }

        lFinalContainer.push_back( lNodeAux );
      }

      preProcessNodeCollection.clear( );
      preProcessNodeCollection = lFinalContainer;
      lFinalContainer.clear( );
    }
  }

  void SWCImporter::SomaBeltTest ( )
  {
    SWCNode lNodeAux;
    OpenMesh::Vec3f lBariCenter( 0, 0, 0 );
    std::vector<SWCNode> lBeltSomaContainer;
    std::vector<SWCNode> lFinalContainer;

    float lDistNode = 10E10;
    unsigned int lNumEles = 0;

    unsigned int lNumNodes = preProcessNodeCollection.size( );
    ;

    unsigned int i = 1;

    while ( preProcessNodeCollection[i].type == SOMAID )
    {
      lBeltSomaContainer.push_back( preProcessNodeCollection[i] );
      ++i;

    }

    lNumEles = lBeltSomaContainer.size( );

    mSomaNodesDiscarded = lBeltSomaContainer.size( ) - 1;

    if ( lNumEles > 1 )
    {

      lNodeAux.id = lNodeAux.type = lNodeAux.position[0] =
          lNodeAux.position[1] = lNodeAux.position[2] = lNodeAux.radius =
              lNodeAux.parent = 0;
      lFinalContainer.push_back( lNodeAux );

      for ( int i = 0; i < lNumEles; i++ )
      {
        lBariCenter += lBeltSomaContainer.at( i ).position;
      }
      lBariCenter /= ( float ) ( lNumEles );

      for ( int i = 0; i < lNumEles; i++ )
      {
        if ( ( lBariCenter - lBeltSomaContainer.at( i ).position ).length( )
            < lDistNode )
        {
          lDistNode =
              ( lBariCenter - lBeltSomaContainer.at( i ).position ).length( );
        }
      }

      lNodeAux = preProcessNodeCollection[1];
      lNodeAux.position = lBariCenter;
      lNodeAux.radius = lDistNode;
      lFinalContainer.push_back( lNodeAux );

      for ( int i = ( lNumEles + 1 ); i < lNumNodes; i++ )
      {
        lNodeAux = preProcessNodeCollection.at( i );

        lNodeAux.id -= ( lNumEles - 1 );

        if ( lNodeAux.type == SOMAID )
        {
          lNodeAux.type = preProcessNodeCollection.at( i + 1 ).type;
          lNodeAux.radius = preProcessNodeCollection.at( i + 1 ).radius;
        }

        if ( lNodeAux.parent <= lNumEles )
        {
          lNodeAux.parent = SOMAID;
        }
        else
        {
          lNodeAux.parent -= ( lNumEles - 1 );
        }

        lFinalContainer.push_back( lNodeAux );
      }

      preProcessNodeCollection.clear( );
      preProcessNodeCollection = lFinalContainer;
      lFinalContainer.clear( );
    }

    lBeltSomaContainer.clear( );
  }

  void SWCImporter::NodeRepetitionTest ( )
  {
    SWCNode lNodeAux;
    SWCNode lNodeAuxParent;

    std::vector<SWCNode> lFinalContainer;
    std::vector<unsigned int> lRepesContainer;

    lFinalContainer.clear( );
    lRepesContainer.clear( );

    unsigned int lNodeToProces = preProcessNodeCollection.size( );
    unsigned int lNodeRepes = 0;

    lRepesContainer.push_back( lNodeRepes );
    lRepesContainer.push_back( lNodeRepes );

    for ( unsigned int i = 2; i < lNodeToProces; ++i )
    {
      lNodeAux = preProcessNodeCollection.at( i );
      lNodeAuxParent = preProcessNodeCollection.at( lNodeAux.parent );

      lRepesContainer.push_back( lNodeRepes );
    }

    lNodeRepes = 0;

    lFinalContainer.push_back( preProcessNodeCollection.at( 0 ) );
    lFinalContainer.push_back( preProcessNodeCollection.at( 1 ) );
    for ( unsigned int i = 2; i < lNodeToProces; ++i )
    {

      lNodeAux = preProcessNodeCollection.at( i );
      lNodeAuxParent = preProcessNodeCollection.at( lNodeAux.parent );
    }

    preProcessNodeCollection.clear( );
    preProcessNodeCollection = lFinalContainer;
    lFinalContainer.clear( );
  }

  void SWCImporter::calcDendriticDistance ( )
  {
    SWCNode lNodeAux;

    float lLocalDistance = 0;
    float lMaxDendriticLong = 0;
    unsigned int lNumNodes = preProcessNodeCollection.size( );

    unsigned int lInicialDendriticNode = 2;
    dendriticSomaConnection.push_back( lInicialDendriticNode );

    for ( int i = 0; i < lNumNodes; ++i )
    {
      preProcessNodeCollection.at( i ).mDendriticDistanceNorm =
          preProcessNodeCollection.at( i ).mDendriticDistanceReal =
              preProcessNodeCollection.at( i ).mDistanceToSoma = 0;
    }

    for ( int i = lInicialDendriticNode; i < lNumNodes; ++i )
    {
      preProcessNodeCollection.at( i ).mDistanceToSoma =
          preProcessNodeCollection[i].position.length( );

      if ( preProcessNodeCollection[i].parent == SOMAID )
      {
        for ( int j = lInicialDendriticNode; j < i; ++j )
        {
          preProcessNodeCollection[j].mDendriticDistanceNorm /=
              lMaxDendriticLong;

          dendriticMaxDistance.push_back( lMaxDendriticLong );
        }

        lMaxDendriticLong = 0;

        lInicialDendriticNode = i;
        dendriticSomaConnection.push_back( lInicialDendriticNode );
      }

      lLocalDistance =
          ( preProcessNodeCollection[i].position
              - preProcessNodeCollection[ ( preProcessNodeCollection[i].parent )].position ).length( );

      preProcessNodeCollection[i].mDendriticDistanceNorm =
          preProcessNodeCollection[i].mDendriticDistanceReal =
              lLocalDistance
                  + preProcessNodeCollection[ ( preProcessNodeCollection[i].parent )].mDendriticDistanceReal;

      if ( lMaxDendriticLong
          < preProcessNodeCollection[i].mDendriticDistanceReal )
      {
        lMaxDendriticLong = preProcessNodeCollection[i].mDendriticDistanceReal;
      }
    }

    for ( int j = lInicialDendriticNode; j < lNumNodes; ++j )
    {
      preProcessNodeCollection[j].mDendriticDistanceNorm /= lMaxDendriticLong;

      dendriticMaxDistance.push_back( lMaxDendriticLong );
    }
  }

  void SWCImporter::calcDendritics ( )
  {
    SWCDendriticInfo lSWCDendriticInfo;
    unsigned int lNumNodes = preProcessNodeCollection.size( );

    unsigned int lInicialDendriticNodeNoSomaConnected = 3;

    lSWCDendriticInfo.initialNode = 2;

    if ( preProcessNodeCollection[lSWCDendriticInfo.initialNode].type == 2 ) lSWCDendriticInfo.type =
        DendriticType::AXON;
    if ( preProcessNodeCollection[lSWCDendriticInfo.initialNode].type == 3 ) lSWCDendriticInfo.type =
        DendriticType::BASAL;
    if ( preProcessNodeCollection[lSWCDendriticInfo.initialNode].type == 4 ) lSWCDendriticInfo.type =
        DendriticType::APICAL;

    for ( int i = lInicialDendriticNodeNoSomaConnected; i < lNumNodes; ++i )
    {
      if ( preProcessNodeCollection[i].parent == SOMAID )
      {
        lSWCDendriticInfo.finalNode = i - 1;

        dendritics.push_back( lSWCDendriticInfo );

        lSWCDendriticInfo.initialNode = i;

        if ( preProcessNodeCollection[lSWCDendriticInfo.initialNode].type == 2 ) lSWCDendriticInfo.type =
            DendriticType::AXON;
        if ( preProcessNodeCollection[lSWCDendriticInfo.initialNode].type == 3 ) lSWCDendriticInfo.type =
            DendriticType::BASAL;
        if ( preProcessNodeCollection[lSWCDendriticInfo.initialNode].type == 4 ) lSWCDendriticInfo.type =
            DendriticType::APICAL;
      }
    }
    lSWCDendriticInfo.finalNode = lNumNodes - 1;

    dendritics.push_back( lSWCDendriticInfo );
  }

  std::string SWCImporter::getDescription ( )
  {
    return description;
  }

  std::string SWCImporter::getContent ( int ini, int fin )
  {
    std::string aux = "---------------------------------\n";

    for ( int i = ini; i < fin; ++i )
    {
      aux += "Id:";
      aux += to_string( nodeCollection[i].id );
      aux += '\n';
      aux += "Type:";
      aux += to_string( nodeCollection[i].type );
      aux += '\n';
      aux += "Values:(";
      aux += to_string( nodeCollection[i].position[0] );
      aux += " , ";
      aux += to_string( nodeCollection[i].position[1] );
      aux += " , ";
      aux += to_string( nodeCollection[i].position[2] );
      aux += ")";
      aux += '\n';
      aux += "Radius:";
      aux += to_string( nodeCollection[i].radius );
      aux += '\n';
      aux += "Parent:";
      aux += to_string( nodeCollection[i].parent );
      aux += '\n';
      aux += "---------------------------------\n";
    }

    return aux;
  }

  unsigned int SWCImporter::getNumNodes ( ) const
  {
    return nodeCollection.size( );
  }

  unsigned int SWCImporter::getNumDendriticTermination ( ) const
  {
    return dendriticTermination.size( );
  }

  unsigned int SWCImporter::getNumDendriticBifurcations ( ) const
  {
    return dendriticBifurcations.size( );
  }

  std::vector<int> SWCImporter::getDendriticBifurcations ( ) const
  {
    return dendriticBifurcations;
  }

  SWCNode SWCImporter::getElementAt ( int i ) const
  {
    if ( i < nodeCollection.size( ) ) return nodeCollection[i];
    else
      return nodeCollection[0];
  }

  void SWCImporter::modifElement ( unsigned int pId, SWCNode pNewValue )
  {
    nodeCollection[pId] = pNewValue;
  }

  void SWCImporter::generateConnectios ( )
  {
    SWCConnection nodeAux;
    for ( int i = 1; i < nodeCollection.size( ); ++i )
    {
      nodeAux.ini = nodeCollection[i].parent;
      nodeAux.fin = nodeCollection[i].id;

      if ( ( nodeAux.ini != -1 ) )
      {
        if ( i + 1 < nodeCollection.size( ) )
        {
          if ( nodeAux.fin == nodeCollection[nodeAux.fin + 1].parent )
          {
            nodeAux.mTermination = false;
            nodeConnections.push_back( nodeAux );

          }
          else
          {
            dendriticTermination.push_back( i );

            nodeAux.mTermination = true;
            nodeConnections.push_back( nodeAux );

          }
        }
        else
        {
          nodeAux.mTermination = false;
          nodeConnections.push_back( nodeAux );

          dendriticTermination.push_back( i );
        }
      }
    }
  }

  void SWCImporter::generateBifurcations ( )
  {
    int lConsecValues = 0;
    SWCConnection lNodeAux;
    for ( unsigned int i = 0; i < nodeConnections.size( ); ++i )
    {
      lNodeAux = nodeConnections.at( i );
      lConsecValues = lNodeAux.fin - lNodeAux.ini;
      if ( ( lConsecValues > 1 ) && ( lNodeAux.ini != SOMAID ) )
      {
        dendriticBifurcations.push_back( lNodeAux.ini );
      }
    }
  }

  void SWCImporter::generateBranches ( )
  {
    SWCBranch lSWCBranch;
    unsigned int lFinalDendId;

    unsigned int lIdCounter;

    for ( int i = 0; i < getNumDendritics( ); ++i )
    {
      lIdCounter = lSWCBranch.initialNode = getDendritics( )[i].initialNode;

      lFinalDendId = getDendritics( )[i].finalNode;

      while ( lIdCounter != lFinalDendId )
      {
        ++lIdCounter;

        if ( isDendriticTermination( lIdCounter )
            || isDendriticBifurcation( lIdCounter ) )
        {
          lSWCBranch.finalNode = lIdCounter;

          if ( ( ( lSWCBranch.initialNode
              - nodeCollection[lSWCBranch.initialNode].parent ) > 1 )
              || ( nodeCollection[lSWCBranch.initialNode].parent == 1 ) )
          {
            lSWCBranch.consecBranch = false;
          }
          else
            lSWCBranch.consecBranch = true;

          mBranches.push_back( lSWCBranch );

          lSWCBranch.initialNode = lIdCounter + 1;
        }
      }
    }
  }

  SWCConnection SWCImporter::getConnectionAt ( int i )
  {
    if ( i < nodeConnections.size( ) ) return nodeConnections[i];
    else
      return nodeConnections[0];
  }

  unsigned int SWCImporter::getNumConnection ( )
  {
    return nodeConnections.size( );
  }

  bool SWCImporter::isDendriticTermination ( int id ) const
  {
    int i = 0;
    int tam = getNumDendriticTermination( );
    bool finded = false;

    while ( ( i < tam ) && ( !finded ) )
    {
      if ( dendriticTermination[i] == id ) finded = true;
      ++i;
    }
    return finded;
  }

  bool SWCImporter::isDendriticBifurcation ( int id ) const
  {
    int i = 0;
    int tam = getNumDendriticBifurcations( );
    bool finded = false;

    while ( ( i < tam ) && ( !finded ) )
    {
      if ( dendriticBifurcations[i] == id ) finded = true;
      ++i;
    }
    return finded;
  }

  const int SWCImporter::getDendriticTermination ( int index ) const
  {
    if ( index < dendriticTermination.size( ) ) return dendriticTermination[index];
    else
      return -1;
  }

  std::vector<int> SWCImporter::getDendriticSomaConnection ( )
  {
    return dendriticSomaConnection;
  }

  std::vector<int> SWCImporter::getDendriticMaxDistance ( )
  {
    return dendriticMaxDistance;
  }

  std::vector<SWCDendriticInfo> SWCImporter::getDendritics ( ) const
  {
    return dendritics;
  }

  unsigned int SWCImporter::getNumDendritics ( )
  {
    return dendritics.size( );
  }

  void SWCImporter::exportToFile ( std::string pFile )
  {
    string lFileTXT = pFile;

    std::ofstream outputFileTXT;

    std::ofstream( lFileTXT.c_str( ) );

    outputFileTXT.open( lFileTXT.c_str( ), std::ios::app );

    outputFileTXT << "# " << "Generated with QtNeuroSynth" << endl;

    SWCNode nodeAux;
    std::cout.precision( 4 );

    nodeAux = nodeCollection.at( 1 );
    outputFileTXT << nodeAux.id << " " << nodeAux.type << " " << "0.0" << " "
        << "0.0" << " " << "0.0" << " " << nodeAux.radius << " "
        << nodeAux.parent << endl;

    for ( int i = 2; i < nodeCollection.size( ); ++i )
    {
      nodeAux = nodeCollection.at( i );

      outputFileTXT << nodeAux.id << " " << nodeAux.type << " "
          << nodeAux.position[0] << " " << nodeAux.position[1] << " "
          << nodeAux.position[2] << " " << nodeAux.radius << " "
          << nodeAux.parent << endl;
    }

    outputFileTXT.close( );
  }

  void SWCImporter::applyUniformModifiers ( float pAxonModifier,
      float pDendriticModifier, float pApicalModifier, float pValBase )
  {
    SWCNode nodeAux;
    bool lParamsApicalCalculated = false;
    bool lAllNodesProcesed = false;
    unsigned int j = 0;

    float lApicalDecrement = 0;
    unsigned int lApicalNumNodes = 0;
    unsigned int lApicalLastNode = 0;

    unsigned int lSuddenDecrementLimit = 0;
    float lSuddenDecrement = 0;
    unsigned int lSuddenLastNode = 0;
    unsigned int lSuddenNumNodes = 0;

    float lSomaRadius = getElementAt( SOMAID ).radius;

    while ( ( !lParamsApicalCalculated ) && ( !lAllNodesProcesed ) )
    {
      nodeAux = nodeCollection.at( j );

      if ( nodeAux.type == 4 )
      {
        bool lApicalNodesFinish = false;

        while ( ( nodeAux.type == 4 ) && ( !lApicalNodesFinish )
            && ( !lAllNodesProcesed ) )
        {
          if ( !isDendriticBifurcation( j ) )
          {
            ++lApicalNumNodes;

            if ( nodeAux.mDendriticDistanceReal
                < ( SOMAAPICALDISTFACTOR * lSomaRadius ) )
            {
              ++lSuddenNumNodes;
              lSuddenDecrementLimit = j;
            }
          }
          else
          {
            if ( lApicalNumNodes != 0 )
            {
              lApicalNodesFinish = true;
              lApicalLastNode = j;
            }
          }

          if ( j == nodeCollection.size( ) - 1 )
          {
            lAllNodesProcesed = true;
          }
          else
          {
            ++j;
            nodeAux = nodeCollection.at( j );
          }
        }

        lSuddenDecrement = ( ( pValBase * APICALRADIUSMODIFSUDDEN )
            - ( pValBase * APICALRADIUSMODIFINITIAL ) )
            / ( lSuddenNumNodes - 1 );

        lApicalDecrement = ( ( pValBase * APICALRADIUSMODIFINITIAL )
            - ( pValBase * APICALRADIUSMODIFINFINAL ) )
            / ( lApicalNumNodes - ( lSuddenNumNodes + 1 ) );

        lParamsApicalCalculated = true;
      }

      if ( j == nodeCollection.size( ) - 1 )
      {
        lAllNodesProcesed = true;
      }

      ++j;
    }

    for ( int i = 1; i < nodeCollection.size( ); ++i )
    {
      nodeAux = nodeCollection.at( i );

      switch ( nodeAux.type )
      {
        case 2:	//* 2 = axon
          nodeCollection.at( i ).radius = pValBase * pAxonModifier;
          break;

        case 3:	//* 3 = dendrite
          nodeCollection.at( i ).radius = pValBase * pDendriticModifier;
          break;

        case 4:	//* 4 = apical dendrite
          if ( i < lApicalLastNode )
          {
            if ( nodeAux.parent == SOMAID )
            {
              nodeCollection.at( i ).radius =
                  pValBase * APICALRADIUSMODIFSUDDEN;
            }
            else
            {
              if ( i <= lSuddenDecrementLimit )
              {
                nodeCollection.at( i ).radius = nodeCollection.at(
                    nodeAux.parent ).radius - lSuddenDecrement;
              }
              else
              {
                nodeCollection.at( i ).radius = nodeCollection.at(
                    nodeAux.parent ).radius - lApicalDecrement;
              }
            }
          }
          else
          {
            nodeCollection.at( i ).radius = pValBase * pApicalModifier;
          }

          break;
      }

    }
  }

  void SWCImporter::appendSWCImporter ( const SWCImporter * pImporter )
  {
    unsigned int lActualNumEles = getNumNodes( );
    unsigned int lNumElesToAppend = pImporter->getNumNodes( );
    SWCNode lNodeAux;

    for ( int i = 2; i < lNumElesToAppend; ++i )
    {
      lNodeAux = pImporter->getElementAt( i );

      if ( lNodeAux.parent == SOMAID )
      {
        lNodeAux.id = lActualNumEles;
      }
      else
      {
        lNodeAux.id += lActualNumEles - 2;
        lNodeAux.parent += lActualNumEles - 2;
      }
      nodeCollection.push_back( lNodeAux );
    }
  }

  bool SWCImporter::isIdInContainer ( SWCNode pId,
      const std::vector<SWCNode> & pVector )
  {

  }
}
