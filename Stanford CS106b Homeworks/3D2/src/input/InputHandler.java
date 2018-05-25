package input;

import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;


public class InputHandler implements KeyListener,FocusListener,MouseListener,MouseMotionListener {
public boolean []key =new boolean[68836] ;
public static int MouseX;
public static int MouseY;
public static int MouseButton;
public static int MouseX2;
public static int MouseY2;
public static boolean MouseExit;

    @Override
    public void keyTyped(KeyEvent e) {
 
    }

    @Override
    public void keyPressed(KeyEvent e) {
        
        int keyCode = e.getKeyCode();
        if(keyCode<0||keyCode<key.length){
            key[keyCode]=true;
        }
    }

    @Override
    public void keyReleased(KeyEvent e) {
        int keyCode=e.getKeyCode();
        if(keyCode<0||keyCode<key.length){
            key[keyCode]=false;
        }
    }

    @Override
    public void focusGained(FocusEvent e) {
 
    }

    @Override
    public void focusLost(FocusEvent e) {
        for(int i =0;i<key.length;i++){
            key[i]=false;
        }
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        MouseButton=e.getButton();
    }

    @Override
    public void mousePressed(MouseEvent e) {
 
    }

    @Override
    public void mouseReleased(MouseEvent e) {
 
    }

    @Override
    public void mouseEntered(MouseEvent e) {
 
    }

    @Override
    public void mouseExited(MouseEvent e) {
        MouseExit=true;
    }

    @Override
    public void mouseDragged(MouseEvent e) {
 
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        MouseX=e.getX();
        MouseY=e.getY();
        
    }
    
}
