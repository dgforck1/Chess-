#include <iostream>
#include <string>
#include <cstring>
#include "ChessGame.h"
#include "Chess.h"

//sdl libraries
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"


#include <ctime>

//prototypes
bool RectClicked(int mX, int mY, Rect&);
std::string MoveString(bool captured, std::string type, int rank, int file,
    int sourceF);

int BoardClicked(int mx, int my, Board &b, Rect &br, int pt);
    //process board being clicked
void BoardReleased(int mx, int my, Board &b, int wpi,
                   Rect &br, int &pt, std::vector< std::string > &Moves,
                   std::vector< std::string > &CapturedWhite,
                   std::vector< std::string > &CapturedBlack);
    //process board released
void QuitClicked();
void ExitClicked();
void DrawClicked();
void ScrollUpClicked();
void ScrollDownClicked(std::vector< std::string > &Moves);


//global vars
bool scroll = false;
int scrollstart = 2,
    scrollend = 31;

void ChessMain(int player)
{
    //used to time code, to be removed later
    time_t timer;
    struct tm y2k;    
    int seconds;
    y2k.tm_hour = 1;   y2k.tm_min = 0; y2k.tm_sec = 0;
    y2k.tm_year = 114; y2k.tm_mon = 0; y2k.tm_mday = 24;
    
  
    
    Surface s(W, H);
    Event event;
    Mouse mouse;

    int mousex = -1,
        mousey = -1,
        workingPieceIndex = -1,        
        option = -1;
    bool clicked = false,
        released = false;


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


    
    
    std::vector< std::string > CapturedWhite;
    std::vector< std::string > CapturedBlack;
    std::vector< std::string > Moves;
    std::vector< DrawPiece > dw;
    
    int playerTurn = 0,
        x = 0,
        y = 0;


    //set rect coords
    exitR.x = W - exitR.w;
    quitR.y = H - quitR.h;

    drawR.x = quitR.x + (drawR.w * 1.5);
    drawR.y = H - drawR.h;

    boardRect.x = 250;
    boardRect.y = 150;


    //initialize draw pieces            
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
                dw.push_back(DrawPiece(&WP, Rect(tempx, tempy, 50, 50)));
                //s.put_image(WP, pieceR);
            }
            else if(tempP.getType() == "R")
            {
                dw.push_back(DrawPiece(&WR, Rect(tempx, tempy, 50, 50)));
                //s.put_image(WR, pieceR);
            }
            else if(tempP.getType() == "N")
            {
                dw.push_back(DrawPiece(&WN, Rect(tempx, tempy, 50, 50)));
                //s.put_image(WN, pieceR);
            }
            else if(tempP.getType() == "B")
            {
                dw.push_back(DrawPiece(&WB, Rect(tempx, tempy, 50, 50)));
                //s.put_image(WB, pieceR);
            }
            else if(tempP.getType() == "Q")
            {
                dw.push_back(DrawPiece(&WQ, Rect(tempx, tempy, 50, 50)));
                //s.put_image(WQ, pieceR);
            }
            else if(tempP.getType() == "K")
            {
                dw.push_back(DrawPiece(&WK, Rect(tempx, tempy, 50, 50)));
                //s.put_image(WK, pieceR);
            }
        }
        else if(tempP.getPlayer() == 1)
        {
            if(tempP.getType() == "P")
            {
                dw.push_back(DrawPiece(&BP, Rect(tempx, tempy, 50, 50)));
                //s.put_image(BP, pieceR);
            }
            else if(tempP.getType() == "R")
            {
                dw.push_back(DrawPiece(&BR, Rect(tempx, tempy, 50, 50)));
                //s.put_image(BR, pieceR);
            }
            else if(tempP.getType() == "N")
            {
                dw.push_back(DrawPiece(&BN, Rect(tempx, tempy, 50, 50)));
                //s.put_image(BN, pieceR);
            }
            else if(tempP.getType() == "B")
            {
                dw.push_back(DrawPiece(&BB, Rect(tempx, tempy, 50, 50)));
                        //s.put_image(BB, pieceR);
            }
            else if(tempP.getType() == "Q")
            {
                dw.push_back(DrawPiece(&BQ, Rect(tempx, tempy, 50, 50)));
                //s.put_image(BQ, pieceR);
            }
            else if(tempP.getType() == "K")
            {
                dw.push_back(DrawPiece(&BK, Rect(tempx, tempy, 50, 50)));
                //s.put_image(BK, pieceR);
            }
        }                                             
    }
    


    
    while(1)
    {
        //get user input
        if(event.poll())
        {        
            if(event.type() == QUIT) break;
            else
            {                                
                if(event.type() == MOUSEBUTTONDOWN)
                {                    
                    /*time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "<<<< clicked at time: "
                    << seconds << std::endl;*/
                    
                    mouse.update(event);                    
                    mousex = mouse.x();
                    mousey = mouse.y();    
                    clicked = true;
                }
                else if(event.type() == MOUSEBUTTONUP)
                {
                    /*time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "<<<< released at time: "
                    << seconds << std::endl;*/
                    
                    mouse.update(event);
                    mousex = mouse.x();
                    mousey = mouse.y();       
                    released = true;
                }
            }
        }
        
        
        
        // here is the first area I'll add to,
        //if havent recieved message then wait

        

        //determine what was clicked, if anything
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


        //determine what to do with what was clicked
        switch(option)
        {
            case -1: //nothing clicked
                break;
            case 0: //game board clicked                                
                if(clicked)
                {
                    /*time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "    <<<< before click process: "
                    << seconds << std::endl;*/
                    
                    workingPieceIndex =
                        BoardClicked(mousex, mousey, b, boardRect,
                                     playerTurn);

                    /*time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "    <<<< after click process: "
                    << seconds << std::endl;*/
                }
                else if(released)
                {
                    /*time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "    <<<< before release process: "
                    << seconds << std::endl;*/
                    
                    BoardReleased(mousex, mousey, b, workingPieceIndex,
                                  boardRect, playerTurn, Moves,
                                  CapturedWhite, CapturedBlack);
                    released = false;
                    workingPieceIndex = -1;
                    

                    /*time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "    <<<< after click process: "
                              << seconds << std::endl;



                    time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "    <<<< before rebuilding dw: "
                    << seconds << std::endl;*/
                    
                    //only update draw pieces if they may have made a move
                    //or later if we receive an update from server
                    
                    dw.clear();
                    
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
                                dw.push_back(
                                    DrawPiece(&WP, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "R")
                            {
                                dw.push_back(
                                    DrawPiece(&WR, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "N")
                            {
                                dw.push_back(
                                    DrawPiece(&WN, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "B")
                            {
                                dw.push_back(
                                    DrawPiece(&WB, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "Q")
                            {
                                dw.push_back(
                                    DrawPiece(&WQ, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "K")
                            {
                                dw.push_back(
                                    DrawPiece(&WK, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                        }
                        else if(tempP.getPlayer() == 1)
                        {
                            if(tempP.getType() == "P")
                            {
                                dw.push_back(
                                    DrawPiece(&BP, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "R")
                            {
                                dw.push_back(
                                    DrawPiece(&BR, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "N")
                            {
                                dw.push_back(
                                    DrawPiece(&BN, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "B")
                            {
                                dw.push_back(
                                    DrawPiece(&BB, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "Q")
                            {
                                dw.push_back(
                                    DrawPiece(&BQ, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                            else if(tempP.getType() == "K")
                            {
                                dw.push_back(
                                    DrawPiece(&BK, Rect(tempx, tempy, 50, 50))
                                    );
                            }
                        }                                             
                    }
                    
                    /*time(&timer);
                    seconds = difftime(timer, mktime(&y2k));
                    std::cout << "    <<<< after rebuilding dw: "
                    << seconds << std::endl;*/
                }
                
                break;
            case 1: //quit button clicked
                QuitClicked();
                break;
            case 2: //exit button clicked
                ExitClicked();
                break;
            case 3: //draw button clicked
                DrawClicked();
                break;
            case 4: //scroll up clicked
                if(scroll)
                {
                    ScrollUpClicked();
                }                
                break;
            case 5: //scroll down clicked
                if(scroll)
                {
                    ScrollDownClicked(Moves);
                }                
                break;                
            default: //heck if i know what was clicked                
                break;
        }


                


        
//###############################################################################
// TODO : MOVE PLACEMENT CALCULATIONS OUTSIDE OF DRAWINGS
//////////////////////////////////////////////////////////////////////////////////

        
        

        /*time(&timer);
        seconds = difftime(timer, mktime(&y2k));
        std::cout << "    <<<< before drawing: "
        << seconds << std::endl;*/
        
        //draw all the things
        s.lock();
        s.fill(GRAY);
        s.put_image(boardI, boardRect);
        s.put_rect(movRect, WHITE);
        s.put_rect(capRect, WHITE);
        s.put_image(WPS, Rect(250, 120, 25, 25));
        s.put_image(BPS, Rect(625, 120, 25, 25));
        //draw pieces
        for(int i = 0; i < dw.size(); i++)
        {
            s.put_image(*dw[i].getImage(), dw[i].getRect());
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



        /*time(&timer);
        seconds = difftime(timer, mktime(&y2k));
        std::cout << "    <<<< after drawing: "
        << seconds << std::endl;*/
        
        //reset variables        
        mousex = -1;        
        mousey = -1;                
        clicked = false;        
        if(option != 0)
        {
            option = -1;
        }
        
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


int BoardClicked(int mx, int my, Board &b, Rect &bR, int pt)
{
    //std::cout << "<<<< board clicked" << std::endl;
                          
    //convert mouse coords to board coords (rank and file)
    int tempx = mx - bR.x;
    int tempy = my - bR.y;
    tempx /= 50;
    tempy /= 50;                
    tempy = 7 - tempy;

    int workingPieceIndex = -1;
    
    workingPieceIndex = b.getPieceIndex(tempy, tempx);
    
    if(workingPieceIndex >= 0)
    {
        Piece tempP = b.getPiece(workingPieceIndex);                
        
        if(tempP.getPlayer() == pt)
        {
            return workingPieceIndex;
        }
    }

    return -1;
}
                                      

void BoardReleased(int mx, int my, Board &b, int wpi, Rect &bR, int &pt,
                   std::vector< std::string > &Moves,
                   std::vector< std::string > &CapturedWhite,
                   std::vector< std::string > &CapturedBlack)
{
    //std::cout << "<<<< board released" << std::endl;


    //convert mouse coords to board coords (rank and file)
    if(wpi >= 0)
    {
        int tempx = mx - bR.x;
        int tempy = my - bR.y;
        tempx /= 50;
        tempy /= 50;                
        tempy = 7 - tempy;
        Piece wp = b.getPiece(wpi);
        
        if(b.checkMove(wpi, tempy, tempx))
        {
            //std::string temp;       
            
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
            
            b.movePiece(wpi, tempy, tempx);         
            
            if(pt == 0)
            {
                pt = 1;
            }
            else
            {
                pt = 0;
            }
        }
    }    
}




void QuitClicked()
{
    //std::cout << "<<<< quick clicked" << std::endl;

}


void ExitClicked()
{
    //std::cout << "<<<< exit clicked" << std::endl;

}


void DrawClicked()
{
    //std::cout << "<<<< draw clicked" << std::endl;
}


void ScrollUpClicked()
{
    //std::cout << "<<<< scroll up clicked" << std::endl;
    if(scrollstart > 0)
    {
        scrollstart -= 2;
        scrollend -= 2;
    }
}


void ScrollDownClicked(std::vector< std::string > &Moves)
{
    //std::cout << "<<<< scroll down clicked" << std::endl;
    if(scrollend < Moves.size() - 1)
    {
        scrollstart += 2;
        scrollend += 2;
    }
}



