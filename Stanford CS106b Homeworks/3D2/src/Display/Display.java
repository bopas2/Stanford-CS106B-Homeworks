package Display;

import input.InputHandler;
import Display.graphics.Render;
import Display.graphics.Screen;
import input.Controller;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.image.BufferStrategy;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.io.File;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.sound.sampled.LineUnavailableException;
import javax.sound.sampled.UnsupportedAudioFileException;
import javax.swing.JFrame;

public class Display extends Canvas implements Runnable {

    public static final int WIDTH = 800;
    public static final int HEIGHT = 600;
    public static final String TITLE = "3D2 Pre-Release Alpha";

    private Thread thread;
    private Screen screen;
    private BufferedImage img;
    private Game game;
    private boolean running = false;
    private Render render;
    private int[] pixels;
    private InputHandler input;
    private int newX = 0;
    private int oldX = 0;
    private int fps;

    public static int MouseSpeed;
    public static boolean mainInvis = true;
    public static Clip clip, clip2;

    public Display() {
        Dimension size = new Dimension(WIDTH, HEIGHT);
        setPreferredSize(size);
        setMinimumSize(size);
        setMaximumSize(size);
        game = new Game();
        screen = new Screen(WIDTH, HEIGHT);
        game = new Game();
        img = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
        pixels = ((DataBufferInt) img.getRaster().getDataBuffer()).getData();

        input = new InputHandler();
        addKeyListener(input);
        addFocusListener(input);
        addMouseListener(input);
        addMouseMotionListener(input);
    }

    private void start() {
        if (running) {
            return;
        }
        running = true;
        thread = new Thread(this);
        thread.start();

    }

    public void run() {
        
        File walkSound = new File("res/sounds/sound.wav");
        File walkSound2 = new File("res/sounds/run.wav");
        
        try {
            clip = AudioSystem.getClip();
            clip.open(AudioSystem.getAudioInputStream(walkSound));
        } catch (LineUnavailableException ex) {
            Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
        } catch (UnsupportedAudioFileException ex) {
            Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        try {
            clip2 = AudioSystem.getClip();
            clip2.open(AudioSystem.getAudioInputStream(walkSound2));
        } catch (LineUnavailableException ex) {
            Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
        } catch (UnsupportedAudioFileException ex) {
            Logger.getLogger(Display.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        int frames = 0;
        double unprocessedSeconds = 0;
        long previousTime = System.nanoTime();
        double secondsPerTick = 1 / 60.0;
        int tickCount = 0;
        boolean ticked = false;
        while (running == true) {
            long currentTime = System.nanoTime();
            long passedTime = currentTime - previousTime;
            previousTime = currentTime;
            unprocessedSeconds += passedTime / 1000000000.0;
            requestFocus();

            while (unprocessedSeconds > secondsPerTick) {
                tick();
                unprocessedSeconds -= secondsPerTick;
                ticked = true;
                tickCount++;
                if (tickCount % 60 == 0) {
                    System.out.println(frames + " fps");
                    fps = frames;
                    previousTime += 1000;
                    frames = 0;
                }
            }
            if (ticked) {
                render();
                frames++;
            }
            render();
            frames++;
            newX = InputHandler.MouseX;
            if (newX > oldX) {

                Controller.turnRight = true;
            }
            if (newX < oldX) {

                Controller.turnLeft = true;
            }
            if (newX == oldX) {

                Controller.turnLeft = false;
                Controller.turnRight = false;
            }
            MouseSpeed = Math.abs(newX - oldX);
            oldX = newX;
        }
    }

    private void tick() {
        game.tick(input.key);
    }

    private void render() {
        BufferStrategy bs = this.getBufferStrategy();
        if (bs == null) {
            createBufferStrategy(3);
            return;
        }

        screen.render(game);

        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            pixels[i] = screen.pixels[i];
        }
        Graphics g = bs.getDrawGraphics();
        g.drawImage(img, 0, 0, WIDTH + 10, HEIGHT + 10, null);
        g.setFont(new Font("Verdana", 2, 30));
        g.setColor(Color.YELLOW);
        g.drawString(fps + " fps", 10, 30);
        g.dispose();
        bs.show();
    }

    private void stop() {
        if (!running) {
            return;
        }
        running = false;
        try {
            thread.join();
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(0);
        }
    }

    public static void main(String[] args) {
        BufferedImage cursor = new BufferedImage(16, 16, BufferedImage.TYPE_INT_ARGB);
        Cursor blank = Toolkit.getDefaultToolkit().createCustomCursor(cursor, new Point(0, 0), "blank");
        Display game = new Display();
        JFrame frame = new JFrame();
        //frame.setUndecorated(true); < removes edges
        frame.add(game);
        frame.pack();
        frame.getContentPane().setCursor(blank);
        frame.setTitle(TITLE);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setResizable(false);
        frame.setAlwaysOnTop(false);
       frame.setVisible(true);
        
        

        System.out.println("Running...");

        game.start();
    }
    public static void imprt(boolean main){
        mainInvis = main;
}
}
