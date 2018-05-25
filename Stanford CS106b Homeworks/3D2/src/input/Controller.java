package input;

import Display.Display;
import static Display.Display.HEIGHT;
import static Display.Display.WIDTH;
import Display.OptionMenu;
import static input.InputHandler.MouseExit;
import static input.InputHandler.MouseX;
import static input.InputHandler.MouseY;
import java.awt.AWTException;
import java.awt.Robot;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;
import static Display.OptionMenu.runSpeed0;
import static Display.OptionMenu.walkSpeed0;
import static Display.OptionMenu.ceilingHeight0;
import static Display.OptionMenu.rednderDistance0;


public class Controller {

    File walkSound = new File("res/sounds/walk.wav");
    File runSound = new File("res/sounds/run.wav");

    public double x, y, z, rotation, xa, za, rotationa;
    public static boolean turnLeft = false;
    public static boolean turnRight = false;

    public static boolean runWalk = false;
    public static boolean walk = false;
    public static boolean crouchWalk = false;
    public static boolean walkSound2 = false;
    public static boolean tabTest = false;
    public static boolean runSound2=false;  
    public static double jumpTimer=0.0,jumpTimer2=0.0;

    
    
    public static int eo=0;
    public static int timer = 0;
    public void tick(boolean foward, boolean back, boolean left, boolean right, boolean jump, boolean crouch, boolean prone, boolean run, boolean quit,boolean tab) {
        OptionMenu o = new OptionMenu();
        double rotationSpeed = .002 * Display.MouseSpeed;
        double walkSpeed = walkSpeed0;
        double jumpHeight = 0.7;
        double xMove = 0;
        double zMove = 0.0;
        double crouchHeight = 0.33;
        double proneHeight = 0.77;
        if (MouseExit) {

        }
        if (quit) {
            System.out.println("Game Closing");
            System.exit(0);
        }

        if (foward) {
            zMove++;
            walk = true;
            walkSound2 = true;
        }

        if (back) {
            zMove--;
            walk = true;
            walkSound2 = true;

        }

        if (left) {
            xMove--;
            walk = true;
            walkSound2 = true;

        }

        if (right) {
            xMove++;
            walk = true;
            walkSound2 = true;

        }

        if (turnLeft) {
            rotationa -= rotationSpeed;

        }

        if (turnRight) {
            rotationa += rotationSpeed;
        }
        if(jumpTimer<15){
        if (jump) {
            jumpTimer+=1;
            y += jumpHeight;
            run = false;
            walk = false;
            walkSound2 = false;
            runSound2=false;
        }}
        if(jumpTimer>=15){
            jumpTimer2+=1;
        }
        if(jumpTimer2>=27){
            jumpTimer=0;
            jumpTimer2 = 0;
        }
        if (crouch) {
            y -= crouchHeight;
            run = false;
            walkSpeed = 0.35;
            crouchWalk = true;
            runSound2=false;
        }
        if (prone) {
            y -= proneHeight;
            run = false;
            walkSpeed = 0.2;
            walk = false;
            walkSound2 = false;
            runSound2=false;

        }
        if (run) {
            walkSpeed = 1;
            walk = true;
            walkSound2 = false;
            runWalk = true;
            runSound2 = true;
        }
        if (!run) {
            runWalk = false;
            runSound2 = false;
        }
        if (!foward && !back && !left && !right) {
            walk = false;
            walkSound2 = false;
            runSound2=false;
        }

        if (!crouch) {
            crouchWalk = false;
        }
        if (prone && crouch) {
            y += crouchHeight;
        }
        
        if(tab){
            
        if(timer<0){
        tabTest = true;
        timer=2000;
        
        
        o.optionsMenu(tabTest);}
        
}
    if(timer<0){
    if(!tab){
        
        tabTest = false;
        o.optionsMenu(tabTest);
        
    }
    }
    if(!tab){
        --timer;
    }

        xa += (xMove * Math.cos(rotation) + zMove * Math.sin(rotation)) * walkSpeed;
        za += (zMove * Math.cos(rotation) - xMove * Math.sin(rotation)) * walkSpeed;

        x += xa;
        y *= 0.9;
        z += za;
        xa *= 0.1;
        za *= 0.1;
        rotation += rotationa;
        rotationa *= .5;

        if (walkSound2) {
            Display.clip.start();

            if (Display.clip.getMicrosecondPosition() == Display.clip.getMicrosecondLength()) {
                Display.clip.stop();
                Display.clip.close();

                try {
                    Display.clip.open(AudioSystem.getAudioInputStream(walkSound));
                } catch (LineUnavailableException ex) {
                    Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IOException ex) {
                    Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
                } catch (UnsupportedAudioFileException ex) {
                    Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        } else {
            Display.clip.stop();
        }
        
        if (runSound2) {
            Display.clip2.start();

            if (Display.clip2.getMicrosecondPosition() == Display.clip2.getMicrosecondLength()) {
                Display.clip2.stop();
                Display.clip2.close();

                try {
                    Display.clip2.open(AudioSystem.getAudioInputStream(runSound));
                } catch (LineUnavailableException ex) {
                    Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IOException ex) {
                    Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
                } catch (UnsupportedAudioFileException ex) {
                    Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        } else {
            Display.clip2.stop();
        }

    }

    private void mouseMove(int x, int y) {

    }

}
