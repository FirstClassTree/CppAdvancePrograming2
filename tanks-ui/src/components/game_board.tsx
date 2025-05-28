import { Grid, Paper, ScrollArea, SimpleGrid } from "@mantine/core";

export default function GameBoard() {
    return (
        <ScrollArea>
            <SimpleGrid cols={3} spacing={0}>
                {Array.from({ length: 9 }, (_, i) => (
                    <Paper
                        style={{
                            border: '1px solid #ccc',
                            height: 40,
                            width: 40,
                            display: 'flex',
                            alignItems: 'center',
                            justifyContent: 'center',
                            fontSize: 22,
                            padding: 0,
                            boxSizing: 'border-box',
                            background: '#f8fafc'
                        }}
                        key={i + 1}>
                        {i + 1}
                    </Paper>
                ))}
            </SimpleGrid>
        </ScrollArea>
    )
}
