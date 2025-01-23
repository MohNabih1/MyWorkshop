// Copyright 2024 by Professor George F. Rice, modifications copyright 2024 by [Mohamad Nabih Alkhateeb]
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

package store;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;

public class Plant extends Product {
    public Plant(String species, Exposure exposure, int price) {
        super("Plant: " + species, price);
        this.exposure = exposure;
    }
 public Plant(BufferedReader reader) throws IOException {
        super(reader);
        this.exposure = Exposure.valueOf(reader.readLine());
    }

    @Override
    public void save(BufferedWriter writer) throws IOException {
        super.save(writer);
        writer.write(exposure.name());
        writer.newLine();
    }


    public Exposure getExposure() {
        return exposure;
    }
    private Exposure exposure;
}
