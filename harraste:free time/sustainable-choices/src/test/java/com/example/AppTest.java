package com.example;

import org.junit.Before;
import org.junit.Test;

import com.example.sustainablechoices.model.ChoicesModel;

import static org.junit.Assert.assertEquals;

public class AppTest {

    private ChoicesModel model;

    @Before
    public void setUp() {
        model = new ChoicesModel();
    }

    @Test
    public void testInitialScore() {
        assertEquals(0, model.getScore());
    }

    @Test
    public void testCalculateScoreWithCar() {
        model.calculateScore("omalla autolla", "sekaruokavalion mukaan", "tuotteita huvinvuoksi");
        assertEquals(5, model.getScore());
    }

    @Test
    public void testCalculateScoreWithPublicTrans() {
        model.calculateScore("julkisilla", "vegaanisesti", "en mitään");
        assertEquals(9, model.getScore());
    }

    @Test
    public void testCalculateScoreWithWalk() {
        model.calculateScore("kävellen tai pyörällä", "kasvisruokavalion mukaan", "tuotteita tarpeeseen");
        assertEquals(10, model.getScore());
    }
}
