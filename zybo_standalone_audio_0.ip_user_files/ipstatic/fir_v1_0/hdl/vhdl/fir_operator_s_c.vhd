-- ==============================================================
-- File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
-- Version: 2016.2
-- Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
-- 
-- ==============================================================

library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity fir_operator_s_c_rom is 
    generic(
             dwidth     : integer := 15; 
             awidth     : integer := 5; 
             mem_size    : integer := 32
    ); 
    port (
          addr0      : in std_logic_vector(awidth-1 downto 0); 
          ce0       : in std_logic; 
          q0         : out std_logic_vector(dwidth-1 downto 0);
          clk       : in std_logic
    ); 
end entity; 


architecture rtl of fir_operator_s_c_rom is 

signal addr0_tmp : std_logic_vector(awidth-1 downto 0); 
type mem_array is array (0 to mem_size-1) of std_logic_vector (dwidth-1 downto 0); 
signal mem : mem_array := (
    0 => "111111111111101", 1 => "111111111101101", 2 => "111111110111000", 
    3 => "111111100111100", 4 => "111111001010110", 5 => "111110100000001", 
    6 => "111101101110011", 7 => "111101000110101", 8 => "111101000011110", 
    9 => "111110000100001", 10 => "000000011110000", 11 => "000100010011101", 
    12 => "001001001011000", 13 => "001110001111110", 14 => "010010011110100", 
    15 to 16=> "010100111000101", 17 => "010010011110100", 18 => "001110001111110", 
    19 => "001001001011000", 20 => "000100010011101", 21 => "000000011110000", 
    22 => "111110000100001", 23 => "111101000011110", 24 => "111101000110101", 
    25 => "111101101110011", 26 => "111110100000001", 27 => "111111001010110", 
    28 => "111111100111100", 29 => "111111110111000", 30 => "111111111101101", 
    31 => "111111111111101" );

attribute syn_rom_style : string;
attribute syn_rom_style of mem : signal is "select_rom";
attribute ROM_STYLE : string;
attribute ROM_STYLE of mem : signal is "distributed";

attribute EQUIVALENT_REGISTER_REMOVAL : string;
begin 


memory_access_guard_0: process (addr0) 
begin
      addr0_tmp <= addr0;
--synthesis translate_off
      if (CONV_INTEGER(addr0) > mem_size-1) then
           addr0_tmp <= (others => '0');
      else 
           addr0_tmp <= addr0;
      end if;
--synthesis translate_on
end process;

p_rom_access: process (clk)  
begin 
    if (clk'event and clk = '1') then
        if (ce0 = '1') then 
            q0 <= mem(CONV_INTEGER(addr0_tmp)); 
        end if;
    end if;
end process;

end rtl;


Library IEEE;
use IEEE.std_logic_1164.all;

entity fir_operator_s_c is
    generic (
        DataWidth : INTEGER := 15;
        AddressRange : INTEGER := 32;
        AddressWidth : INTEGER := 5);
    port (
        reset : IN STD_LOGIC;
        clk : IN STD_LOGIC;
        address0 : IN STD_LOGIC_VECTOR(AddressWidth - 1 DOWNTO 0);
        ce0 : IN STD_LOGIC;
        q0 : OUT STD_LOGIC_VECTOR(DataWidth - 1 DOWNTO 0));
end entity;

architecture arch of fir_operator_s_c is
    component fir_operator_s_c_rom is
        port (
            clk : IN STD_LOGIC;
            addr0 : IN STD_LOGIC_VECTOR;
            ce0 : IN STD_LOGIC;
            q0 : OUT STD_LOGIC_VECTOR);
    end component;



begin
    fir_operator_s_c_rom_U :  component fir_operator_s_c_rom
    port map (
        clk => clk,
        addr0 => address0,
        ce0 => ce0,
        q0 => q0);

end architecture;


