#include <iostream>
#include <string>
#include <cstring>
#include "ChessGame.h"
#include "Chess.h"

//sdl libraries
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"



//prototypes
bool RectClicked(int mX, int mY, Rect&);
std::string MoveString(bool captured, std::string type, int rank, int file,
    int sourceF);

void BoardClicked(int mx, int my, Board &b);  //


void ChessMain(int player)
{
    
    Surface s(W, H);
    Event event;
    Mouse mouse;

    int mousex = -1,
        mousey = -1,
        workingPieceIndex = -1,
        scrollstart = 2,
        scrollend = 31,
        option = -1;
    bool clicked = false,
        released = false,
        scroll = false;


    Board b = Board();


    
    Image WP = Image("images/WhitePawn.png");
    Image WR = Image("images/WhiteRook.png");
    Image WN = Image("images/WhiteKnight.png");
    Image WB = Image("images/WhiteBishop.png");
    Image WQ = Image("images/WhiteQueen.png");
    Image WK = Image("images/WhiteKing.png");    
    Image BP = Image("images/BlackPawn.png");
    Image BR = Image("images/BlackRook.png");
    Image BN = Image("images/BlackKnight.png");
    Image BB = Image("images/BlackBishop.png");
    Image BQ = Image("images/BlackQueen.png");
    Image BK = Image("images/BlackKing.png");    
    Image WPS = Image("images/WhitePawn-Small.png");
    Image BPS = Image("images/BlackPawn-Small.png");
    Image WRS = Image("images/WhiteRook-Small.png");
    Image WNS = Image("images/WhiteKnight-Small.png");
    Image WBS = Image("images/WhiteBishop-Small.png");
    Image WQS = Image("images/WhiteQueen-Small.png");
    Image WKS = Image("images/WhiteKing-Small.png");        
    Image BRS = Image("images/BlackRook-Small.png");
    Image BNS = Image("images/BlackKnight-Small.png");
    Image BBS = Image("images/BlackBishop-Small.png");
    Image BQS = Image("images/BlackQueen-Small.png");
    Image BKS = Image("images/BlackKing-Small.png");
    Image SUP = Image("images/ScrollUp.png");
    Image SDW = Image("images/ScrollDown.png");
    Image quitI = Image("images/Quit.png");
    Image drawI = Image("images/Draw.png");
    Image exitI = Image("images/Exit.png");
    Image boardI = Image("images/Board.png");
    
    

    Rect boardRect = boardI.getRect(),
        blackRect = Rect(0, 0, 50, 50),
        capRect = Rect(25, 150, 200, 400),
        movRect = Rect(675, 150, 200, 400),
        supRect = Rect(850, 150, 25, 25),
        sdwRect = Rect(850, 525, 25, 25),
        quitR = quitI.getRect(),
        drawR = drawI.getRect(),
        exitR = exitI.getRect(),
        pieceR = Rect(0, 0, 50, 50);


    //set rect coords
    exitR.x = W - exitR.w;
    quitR.y = H - quitR.h;

    drawR.x = quitR.x + (drawR.w * 1.5);
    drawR.y = H - drawR.h;

    boardRect.x = 250;
    boardRect.y = 150;
    
    std::vector< std::string > CapturedWhite;
    std::vector< std::string > CapturedBlack;
    std::vector< std::string > Moves;
    
    int playerTurn = 0;    
    
    
    while(1)
    {
        //std::cout << "<<<< entered while statement" << std::endl;
        if(event.poll())
        {        
            if(event.type() == QUIT) break;
            else
            {
                if(event.type() == MOUSEBUTTONDOWN)
                {
                    mouse.update(event);                    
                    mousex = mouse.x();
                    mousey = mouse.y();                                        
                    clicked = true;
                }
                else if(event.type() == MOUSEBUTTONUP)
                {
                    mouse.update(event);
                    mousex = mouse.x();
                    mousey = mouse.y();                                       
                    released = true;
                }
            }
        }

        
        
// here is the first area I'll add to, if havent recieved message then wait
        //if(playerTurn == player)
            //only cares if it's the current player's turn
        //{
            //player operating on board


        if(clicked)
        {
            if(RectClicked(mousex, mousey, boardRect))
            {
                option = 0;
            }
            else if(RectClicked(mousex, mousey, quitR))
            {
                option = 1;
            }
            else if(RectClicked(mousex, mousey, exitR))
            {
                option = 2;
            }
            else if(RectClicked(mousex, mousey, drawR))
            {
                option = 3;
            }
            else if(RectClicked(mousex, mousey, supRect))
            {
                option = 4;
            }
            else if(RectClicked(mousex, mousey, sdwRect))
            {
                option = 5;
            }
        }


        switch(option)
        {
            case -1: //nothing clicked
                break;
            case 0: //game board clicked                
                break;
            case 1: //quit button clicked
                break;
            case 2: //exit button clicked
                break;
            case 3: //draw button clicked
                break;
            case 4: //scroll up clicked
                break;
            case 5: //scroll down clicked
                break;                
            default: //heck if i know what was clicked
                break;
        }



        
            if(RectClicked(mousex, mousey, boardRect))                
            {                
                
                if(clicked && workingPieceIndex == -1)
                {
                    //std::cout << "<<<< grabbing a working piece" << std::endl;
                    //convert mouse coords to board coords (rank and file)
                    int tempx = mousex - boardRect.x;
                    int tempy = mousey - boardRect.y;
                    tempx /= 50;
                    tempy /= 50;                
                    tempy = 7 - tempy;
                    
                    workingPieceIndex = b.getPieceIndex(tempy, tempx);
                    
                    if(workingPieceIndex >= 0)
                    {
                        //std::cout << "<<<< working piece: ";
                        Piece tempP = b.getPiece(workingPieceIndex);

                        /*tempP.print();
                          std::cout << std::endl;*/
                        
                        if(tempP.getPlayer()  != playerTurn)
                        {
                            //not the current player's piece
                            workingPieceIndex = -1;                    
                        }
                    }
                }

                //std::cout << "<<<< passed board clicked" << std::endl;
                
                if(released)
                {
                    //std::cout << "<<<< enetered board released" << std::endl;
                    //convert mouse coords to board coords (rank and file)
                    if(workingPieceIndex >= 0)
                    {
                        int tempx = mousex - boardRect.x;
                        int tempy = mousey - boardRect.y;
                        tempx /= 50;
                        tempy /= 50;                
                        tempy = 7 - tempy;
                        Piece wp = b.getPiece(workingPieceIndex);
                        
                        if(b.checkMove(workingPieceIndex, tempy, tempx))
                        {
                            std::string temp;       
                            
                            if(b.getPieceIndex(tempy, tempx) >= 0)
                            {
                                int temp = b.getPieceIndex(tempy, tempx);
                                
                                
                                Piece tempP = b.getPiece(tempy, tempx);
                                
                                Moves.push_back(
                                    MoveString(true, wp.getType(),
                                               tempy, tempx, wp.getFile())
                                    );
                                
                                //todo: fix weird capture bug
                                if(tempP.getPlayer() == 0)
                                {
                                    CapturedWhite.push_back(
                                        b.capturePiece(temp)
                                        );
                                }
                                else
                                {
                                    CapturedBlack.push_back(               
                                        b.capturePiece(temp)
                                        );
                                }
                                
                            }
                            else
                            {
                                Moves.push_back(
                                    MoveString(false, wp.getType(),
                                               tempy, tempx, wp.getFile())
                                    );
                            }
                            
                            if(Moves.size() > 30)
                            {
                                scroll = true;
                                scrollstart = Moves.size() - 30 -
                                    (Moves.size() % 2);
                                scrollend = Moves.size() -
                                    (Moves.size() % 2);                     
                            }
                            
                            b.movePiece(workingPieceIndex, tempy, tempx);         
                            
                            if(playerTurn == 0)
                            {
                                playerTurn = 1;
                            }
                            else
                            {
                                playerTurn = 0;
                            }
                        }
                    }
                    
                    //reset variables
                    workingPieceIndex = -1;
                    clicked = false;
                    released = false;
                }
            }
            else
            {
                //std::cout << "<<<< debug point 10" << std::endl;
                
                if(clicked)
                {
                    if(RectClicked(mousex, mousey, quitR))
                    {
                        std::cout << "<<<< quit clicked" << std::endl;
                    }
                    else if(RectClicked(mousex, mousey, drawR))
                    {
                        std::cout << "<<<< draw clicked" << std::endl;
                    }
                    else if(scroll)
                    {
                        if(RectClicked(mousex, mousey, supRect))
                        {
                            if(scrollstart > 0)
                            {
                                scrollstart -= 2;
                                scrollend -= 2;
                            }                                          
                        }
                        
                        if(RectClicked(mousex, mousey, sdwRect))
                        {
                            if(scrollend < Moves.size() - 1)
                            {
                                scrollstart += 2;
                                scrollend += 2;
                            }                                        
                        }                    
                    }
                    clicked = false;
                }                                
            }
                
            //}


        //reset mouse variables
        if(mousex != -1)
        {
            mousex = -1;
        }

        if(mousey != -1)
        {
            mousey = -1;
        }
        

        int toggle = -1;
        int x = 0, y = 0;

        //std::cout << "<<<< debug point 8" << std::endl;
        
        //draw all the things
        s.lock();
        s.fill(GRAY);
        //s.put_rect(boardRect, WHITE);
        s.put_image(boardI, boardRect);
        s.put_rect(movRect, WHITE);
        s.put_rect(capRect, WHITE);
        s.put_image(WPS, Rect(250, 120, 25, 25));
        s.put_image(BPS, Rect(625, 120, 25, 25));
        //draw squares
        /*for(int i = boardRect.x; i < boardRect.x + boardRect.w ; i+= 50)
        {
            for(int n = boardRect.y; n < boardRect.y + boardRect.h; n+= 50)
            {
                blackRect.x = i;
                blackRect.y = n;

                if(toggle == 1)
                {
                    s.put_rect(blackRect, RED);
                }
                
                toggle *= -1;
            }
            toggle *= -1;
            }*/
        //draw pieces
        for(int i = 0; i < b.getPieceSize(); i++)
        {            
            Piece tempP = b.getPiece(i);

            int tempx = boardRect.x;
            int tempy = boardRect.y;            

            
            tempx += tempP.getFile() * 50;
            tempy += ((7 - tempP.getRank()) * 50);

            pieceR.x = tempx;
            pieceR.y = tempy;
            
            if(tempP.getPlayer() == 0)
            {
                if(tempP.getType() == "P")
                {
                    //s.put_image(WP, Rect(tempx, tempy, 50, 50));
                    s.put_image(WP, pieceR);
                }
                else if(tempP.getType() == "R")
                {                    
                    s.put_image(WR, pieceR);
                }
                else if(tempP.getType() == "N")
                {                    
                    s.put_image(WN, pieceR);
                }
                else if(tempP.getType() == "B")
                {                    
                    s.put_image(WB, pieceR);
                }
                else if(tempP.getType() == "Q")
                {                    
                    s.put_image(WQ, pieceR);
                }
                else if(tempP.getType() == "K")
                {                    
                    s.put_image(WK, pieceR);
                }
            }
            else if(tempP.getPlayer() == 1)
            {
                if(tempP.getType() == "P")
                {                    
                    s.put_image(BP, pieceR);
                }
                else if(tempP.getType() == "R")
                {                    
                    s.put_image(BR, pieceR);
                }
                else if(tempP.getType() == "N")
                {                    
                    s.put_image(BN, pieceR);
                }
                else if(tempP.getType() == "B")
                {                    
                    s.put_image(BB, pieceR);
                }
                else if(tempP.getType() == "Q")
                {                    
                    s.put_image(BQ, pieceR);
                }
                else if(tempP.getType() == "K")
                {                    
                    s.put_image(BK, pieceR);
                }
            }
        }

        //draw turn indicator
        if(playerTurn == 0)
        {
            s.put_circle(250, 125, 5, GREEN);
        }
        else
        {
            s.put_circle(650, 125, 5, GREEN);
        }
        
        //draw captured white pieces
        x = 25;
        y = 150;
        for(int i = 0; i < CapturedWhite.size(); i++)
        {
            if(CapturedWhite[i] == "P")
            {
                s.put_image(WPS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "R")
            {
                s.put_image(WRS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "N")
            {
                s.put_image(WNS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "B")
            {
                s.put_image(WBS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "Q")
            {
                s.put_image(WQS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "K")
            {
                s.put_image(WKS, Rect(x + (i * 25), y, 25, 25));
            }
            if(x + (i * 25) >= 200)
            {
                x -= ((i * 25) + 25);
                y += 25;
            }
        }
        //draw captured black pieces
        x = 25;
        y = 350;
        for(int i = 0; i < CapturedBlack.size(); i++)
        {
            if(CapturedBlack[i] == "P")
            {
                s.put_image(BPS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "R")
            {
                s.put_image(BRS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "N")
            {
                s.put_image(BNS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "B")
            {
                s.put_image(BBS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "Q")
            {
                s.put_image(BQS, Rect(x + (i * 25), y, 25, 25));
            }
            else if(CapturedWhite[i] == "K")
            {
                s.put_image(BKS, Rect(x + (i * 25), y, 25, 25));
            }
            if(x + (i * 25) >= 200)
            {
                x -= ((i * 25) + 25);
                y += 25;
            }
        }
        //print moves
        if(scroll)
        {
            x = 675;
            y = 150;
            
            for(int i = scrollstart; i <= scrollend; i++)
            {
                TextSurface ts = TextSurface(
                    Moves[i].c_str(), "fonts/FreeSans.ttf", 16, 0, 0, 0);
                
                s.put_text(ts, x, y);
                if(i % 2 == 1)
                {
                    x = 675;
                    y += 25;
                }
                else
                {
                    x += 100;
                }
            }

            s.put_image(SUP, supRect);
            s.put_image(SDW, sdwRect);
        }        
        else
        {
            x = 675;
            y = 150;
            for(int i = 0; i < Moves.size(); i++)
            {
                TextSurface ts = TextSurface(
                    Moves[i].c_str(), "fonts/FreeSans.ttf", 16, 0, 0, 0);
                
                s.put_text(ts, x, y);
                if(i % 2 == 1)
                {
                    x = 675;
                    y += 25;
                }
                else
                {
                    x += 100;
                }                
            }
        }
        s.put_image(quitI, quitR);
        s.put_image(drawI, drawR);
        s.put_image(exitI, exitR);
        s.unlock();
        s.flip();
        
        delay(10);
        //std::cout << "<<<< debug point 7" << std::endl;
    }
    
        
    //todo:
    //decrement clock, if applicable
}


bool RectClicked(int mX, int mY, Rect& r)
{
    if(mX >= r.x && mX <= r.x + r.w)
    {
        if(mY >= r.y && mY <= r.y + r.h)
        {
            return true;
        }
    }
    
    return false;
}

//build the move string
std::string MoveString(bool captured, std::string type, int rank, int file,
    int sourceF)
{
    std::string temp;

    if(type != "P")
    {
        temp.append(type);
    }

    if(captured)
    {
        if(type == "P")
        {
            switch(sourceF)
            {
                case 0:
                    temp.append("a");
                    break;
                case 1:
                    temp.append("b");
                    break;
                case 2:
                    temp.append("c");
                    break;
                case 3:
                    temp.append("d");
                    break;
                case 4:
                    temp.append("e");
                    break;
                case 5:
                    temp.append("f");
                    break;
                case 6:
                    temp.append("g");
                    break;
                case 7:
                    temp.append("h");
                    break;
            }
        }
        
        temp.append("x");
    }

    switch(file)
    {
        case 0:
            temp.append("a");
            break;
        case 1:
            temp.append("b");
            break;
        case 2:
            temp.append("c");
            break;
        case 3:
            temp.append("d");
            break;
        case 4:
            temp.append("e");
            break;
        case 5:
            temp.append("f");
            break;
        case 6:
            temp.append("g");
            break;
        case 7:
            temp.append("h");
            break;
    }
    
    switch(rank)
    {
        case 0:
            temp.append("1");
            break;
        case 1:
            temp.append("2");
            break;
        case 2:
            temp.append("3");
            break;
        case 3:
            temp.append("4");
            break;
        case 4:
            temp.append("5");
            break;
        case 5:
            temp.append("6");
            break;
        case 6:
            temp.append("7");
            break;
        case 7:
            temp.append("8");
            break;
    }
    
    return temp;
}
