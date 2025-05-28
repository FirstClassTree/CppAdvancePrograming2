import { useState } from "react";
import { AppShell, Center, Text } from "@mantine/core";
import GameBoard from "./components/game_board";

function App() {
  return (<AppShell
  header={{height: 60}}
  navbar={{width: 300, breakpoint: 'sm'}}
  padding="md"
  >
    <AppShell.Header>
      <Text>Header</Text>
    </AppShell.Header>
    <AppShell.Main>
      <Center>
        <GameBoard />
      </Center>
          </AppShell.Main>
    <AppShell.Navbar>
      <Text>Navbar</Text>
    </AppShell.Navbar>
  </AppShell>
  )
}

export default App;
